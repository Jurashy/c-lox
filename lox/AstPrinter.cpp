#include "AstPrinter.hpp"
#include "../output/Expr.hpp"
#include "TokenType.hpp"
#include "Token.hpp"

#include <vector>
#include <string>
#include <type_traits>
#include <iostream>
#include <variant>


auto AstPrinter::visitPrintStmt(Print& stmt) -> Value {
    return std::string("print stmt");
}

auto AstPrinter::visitExpressionStmt(Expression& stmt) -> Value {
    std::cout << "executing print\n";
    return std::string(" expression stmt");
}

auto AstPrinter::print(Expr& expr) -> std::string 
{
    return std::any_cast<std::string>(expr.accept(*this));
}

auto AstPrinter::visitBinaryExpr(Binary& expr) -> Value
{
    return parenthesize(expr.my_operator.getLexeme(), expr.left, expr.right);
}


auto
AstPrinter::visitGroupingExpr(Grouping& expr) -> Value
{
    return parenthesize("group", expr.expression);
}

auto AstPrinter::visitLLiteralExpr(LLiteral& expr) -> Value
{
    return std::visit([](auto&& val) -> std::string {
        using T = std::decay_t<decltype(val)>;

        if constexpr (std::is_same_v<T, std::monostate>) 
            return std::string("nil");
        else if constexpr (std::is_same_v<T, std::string>)
            return val;
        else if constexpr (std::is_same_v<T, bool>)
            return val ? "true" : "false";
        else if constexpr (std::is_same_v<T, std::shared_ptr<LoxCallable>>)
            return "<fn>";
        //else if constexpr (std::is_same_v<T, std::shared_ptr<LoxClass>>) return val.toString();
        else return "unknown";

    }, expr.value);
}

//page 73

auto
AstPrinter::visitUnaryExpr(Unary& expr) -> Value
{
    return parenthesize(expr.my_operator.getLexeme(), expr.right);
}


//
template <class... Exprs> auto 
AstPrinter::parenthesize(std::string name, Exprs&... exprs) -> std::string
{
    std::string builder = "(" + name;

    //fold expr
    ((builder += " " + std::any_cast<std::string>(exprs->accept(*this))), ...);

    builder+= ")";
    return builder;
}

// auto main(int argc, char** argv) -> int
// {
//     std::unique_ptr<Expr> expression =
//         std::make_unique<Binary>(
//             std::make_unique<Unary>(
//                 Token(TokenType::MINUS, "-", std::monostate{}, 1),
//                 std::make_unique<LLiteral>(123)
//             ),
//             Token(TokenType::STAR, "*", std::monostate{}, 1),
//             std::make_unique<Grouping>(
//                 std::make_unique<LLiteral>(45.67)
//             )
//         );

//     AstPrinter ast_printer;
//     std::cout << ast_printer.print(*expression);
// }



//76