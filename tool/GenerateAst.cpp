#include "GenerateAst.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <format>

std::vector<Str> as_list
    {
        "Assign     : Token name, std::shared_ptr<Expr> value",
        "Binary        : std::shared_ptr<Expr> left, Token my_operator, std::shared_ptr<Expr> right",
        "Grouping   : std::shared_ptr<Expr> expression",
        "LLiteral   : Value value",
        "Unary      : Token my_operator, std::shared_ptr<Expr> right",
        "Variable   : Token name"
    };
std::vector<Str> Statements
{
    "Block      : std::vector<std::shared_ptr<Stmt>> statements",
    "Expression    : std::shared_ptr<Expr> expression",
    "Print      : std::shared_ptr<Expr> expression",
    "Var        : Token name, std::shared_ptr<Expr> initializer"
};

auto main(int argc, char** argv) -> int
{
    if (argc != 2) 
    {
        std::cerr << "Usage: generate_ast <output directory>\n";
        std::exit(EXIT_FAILURE);
    }
    std::string outputDir = argv[1];


    Str Expr = "Expr";
    Str Stmt = "Stmt";
    defineAst(outputDir, Expr, as_list, Expr);

    defineAst(outputDir, Stmt, Statements, Stmt);
    std::cout << "argc = " << argc << "\n";

}
//

//
static auto 
defineAst(Str& outputDir, Str& baseName, std::vector<Str>& types, Str __type__name) -> void
{
    Str Visitor = baseName + "Visitor";
    Str path = outputDir + "/" + baseName + ".hpp";
    std::ofstream writer(path);

    writer << 
    std::format("#pragma once\n#include <memory>\n#include <any>\n#include <variant>\n#include <vector> \n") << "#include \"../lox/Token.hpp\"\n";
    writer << "#include \"../lox/Value.hpp\"\n";

    if (baseName == "Stmt") {
        writer << "#include \"Expr.hpp\"\n";
    }
    for (Str type : types) {
        size_t pos = type.find(':');
        Str className = trim(type.substr(0, pos));
        writer << "struct " << className << ";\n";
    }



    defineVisitor(writer, baseName, types, __type__name); //
    writer << "struct " << baseName
       << " {\nvirtual ~" << baseName
       << "() = default;\n"
       << "virtual Value accept( "
        << baseName << "Visitor& visitor) = 0;\n};\n";

    // the AST classes
    //writer << "template <class T>\n";
    for (Str type : types)
    {
        size_t pos = type.find(':');
        Str className = trim(type.substr(0, pos));
        Str fields = trim(type.substr(pos + 1));

        if (className == "ExprVisitor" || className == "StmtVisitor")
            writer << "virtual ~" << className << "() = default;\n";
        defineType(writer, baseName, Visitor, className, fields);
    }

    // the base accept method
    writer.close();
}


//page 66


static auto  
defineType(std::ofstream& writer, Str& baseName, Str& visitorName ,Str& className, Str& fieldList)
     -> void
{
    Str Visitor = baseName + "Visitor";  // ExprVisitor or StmtVisitor

    writer << std::format("struct {} : public {}", className, baseName) << "\n{\n"
    //constructor
    << std::format("    {} ( {} ) \n\t: ", className, fieldList);

    Str delimiter = ", ";
    std::vector<Str> fields = split__(fieldList, delimiter);
    
    for (size_t i {0}; i < fields.size(); ++i)
    {
        Str name = splitIndexOne(fields[i]);
            
        writer << std::format("\t{} ({})", name, name);

        if (i + 1 < fields.size()) writer << ",\n";
    }
    writer << "{}";
    writer  << "\n";

    for (Str& field : fields)
    {
        writer << "\t " + field << ";";
    }
    // here
    writer << "\n   Value accept(" << Visitor <<  " &visitor) override {\n"
    << "    return visitor.visit" << className  << baseName << "(*this);\n"
    << "    }\n";

    writer << "\n};\n";
}

static auto 
defineVisitor(PrintWriter& writer, Str& baseName, std::vector<Str>& types, Str __type__name)
    -> void
{
    Str Visitor = baseName + "Visitor";
    writer << "struct " << Visitor << "\n{\n";
    
    for (Str type : types)
    {
        size_t pos = type.find(':');
        Str typeName = trim(type.substr(0, pos));
        Str smartPtr = "std::unique_ptr<" + __type__name + ">";
        writer << "     virtual Value visit" + typeName + baseName + "( " + typeName + " &expr) = 0;\n ";
    }
    writer << "\n};\n";
}