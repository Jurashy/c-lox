#pragma once
#include <memory>
#include <any>
#include <variant>
#include <vector> 
#include "../lox/Token.hpp"
#include "../lox/Value.hpp"
#include "Expr.hpp"
struct Block;
struct Expression;
struct Class;
struct Function;
struct IF;
struct Print;
struct Return;
struct Var;
struct While;
struct StmtVisitor
{
     virtual Value visitBlockStmt( Block &expr) = 0;
      virtual Value visitExpressionStmt( Expression &expr) = 0;
      virtual Value visitClassStmt( Class &expr) = 0;
      virtual Value visitFunctionStmt( Function &expr) = 0;
      virtual Value visitIFStmt( IF &expr) = 0;
      virtual Value visitPrintStmt( Print &expr) = 0;
      virtual Value visitReturnStmt( Return &expr) = 0;
      virtual Value visitVarStmt( Var &expr) = 0;
      virtual Value visitWhileStmt( While &expr) = 0;
 
};
struct Stmt {
virtual ~Stmt() = default;
virtual Value accept( StmtVisitor& visitor) = 0;
};
struct Block : public Stmt
{
    Block ( std::vector<std::shared_ptr<Stmt>> statements ) 
	: 	statements (statements){}
	 std::vector<std::shared_ptr<Stmt>> statements;
   Value accept(StmtVisitor &visitor) override {
    return visitor.visitBlockStmt(*this);
    }

};
struct Expression : public Stmt
{
    Expression ( std::shared_ptr<Expr> expression ) 
	: 	expression (expression){}
	 std::shared_ptr<Expr> expression;
   Value accept(StmtVisitor &visitor) override {
    return visitor.visitExpressionStmt(*this);
    }

};
struct Class : public Stmt
{
    Class ( Token name, std::vector<std::shared_ptr<Function>> methods ) 
	: 	name (name),
	methods (methods){}
	 Token name;	 std::vector<std::shared_ptr<Function>> methods;
   Value accept(StmtVisitor &visitor) override {
    return visitor.visitClassStmt(*this);
    }

};
struct Function : public Stmt
{
    Function ( Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body ) 
	: 	name (name),
	params (params),
	body (body){}
	 Token name;	 std::vector<Token> params;	 std::vector<std::shared_ptr<Stmt>> body;
   Value accept(StmtVisitor &visitor) override {
    return visitor.visitFunctionStmt(*this);
    }

};
struct IF : public Stmt
{
    IF ( std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch ) 
	: 	condition (condition),
	thenBranch (thenBranch),
	elseBranch (elseBranch){}
	 std::shared_ptr<Expr> condition;	 std::shared_ptr<Stmt> thenBranch;	 std::shared_ptr<Stmt> elseBranch;
   Value accept(StmtVisitor &visitor) override {
    return visitor.visitIFStmt(*this);
    }

};
struct Print : public Stmt
{
    Print ( std::shared_ptr<Expr> expression ) 
	: 	expression (expression){}
	 std::shared_ptr<Expr> expression;
   Value accept(StmtVisitor &visitor) override {
    return visitor.visitPrintStmt(*this);
    }

};
struct Return : public Stmt
{
    Return ( Token keyword, std::shared_ptr<Expr> value ) 
	: 	keyword (keyword),
	value (value){}
	 Token keyword;	 std::shared_ptr<Expr> value;
   Value accept(StmtVisitor &visitor) override {
    return visitor.visitReturnStmt(*this);
    }

};
struct Var : public Stmt
{
    Var ( Token name, std::shared_ptr<Expr> initializer ) 
	: 	name (name),
	initializer (initializer){}
	 Token name;	 std::shared_ptr<Expr> initializer;
   Value accept(StmtVisitor &visitor) override {
    return visitor.visitVarStmt(*this);
    }

};
struct While : public Stmt
{
    While ( std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body ) 
	: 	condition (condition),
	body (body){}
	 std::shared_ptr<Expr> condition;	 std::shared_ptr<Stmt> body;
   Value accept(StmtVisitor &visitor) override {
    return visitor.visitWhileStmt(*this);
    }

};
