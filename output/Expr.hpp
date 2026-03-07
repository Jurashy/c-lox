#pragma once
#include <memory>
#include <any>
#include <variant>
#include <vector> 
#include "../lox/Token.hpp"
#include "../lox/Value.hpp"
struct Assign;
struct Binary;
struct Call;
struct Grouping;
struct LLiteral;
struct Logical;
struct Unary;
struct Variable;
struct ExprVisitor
{
     virtual Value visitAssignExpr( Assign &expr) = 0;
      virtual Value visitBinaryExpr( Binary &expr) = 0;
      virtual Value visitCallExpr( Call &expr) = 0;
      virtual Value visitGroupingExpr( Grouping &expr) = 0;
      virtual Value visitLLiteralExpr( LLiteral &expr) = 0;
      virtual Value visitLogicalExpr( Logical &expr) = 0;
      virtual Value visitUnaryExpr( Unary &expr) = 0;
      virtual Value visitVariableExpr( Variable &expr) = 0;
 
};
struct Expr {
virtual ~Expr() = default;
virtual Value accept( ExprVisitor& visitor) = 0;
};
struct Assign : public Expr
{
    Assign ( Token name, std::shared_ptr<Expr> value ) 
	: 	name (name),
	value (value){}
	 Token name;	 std::shared_ptr<Expr> value;
   Value accept(ExprVisitor &visitor) override {
    return visitor.visitAssignExpr(*this);
    }

};
struct Binary : public Expr
{
    Binary ( std::shared_ptr<Expr> left, Token my_operator, std::shared_ptr<Expr> right ) 
	: 	left (left),
	my_operator (my_operator),
	right (right){}
	 std::shared_ptr<Expr> left;	 Token my_operator;	 std::shared_ptr<Expr> right;
   Value accept(ExprVisitor &visitor) override {
    return visitor.visitBinaryExpr(*this);
    }

};
struct Call : public Expr
{
    Call ( std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments ) 
	: 	callee (callee),
	paren (paren),
	arguments (arguments){}
	 std::shared_ptr<Expr> callee;	 Token paren;	 std::vector<std::shared_ptr<Expr>> arguments;
   Value accept(ExprVisitor &visitor) override {
    return visitor.visitCallExpr(*this);
    }

};
struct Grouping : public Expr
{
    Grouping ( std::shared_ptr<Expr> expression ) 
	: 	expression (expression){}
	 std::shared_ptr<Expr> expression;
   Value accept(ExprVisitor &visitor) override {
    return visitor.visitGroupingExpr(*this);
    }

};
struct LLiteral : public Expr
{
    LLiteral ( Value value ) 
	: 	value (value){}
	 Value value;
   Value accept(ExprVisitor &visitor) override {
    return visitor.visitLLiteralExpr(*this);
    }

};
struct Logical : public Expr
{
    Logical ( std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right ) 
	: 	left (left),
	oper (oper),
	right (right){}
	 std::shared_ptr<Expr> left;	 Token oper;	 std::shared_ptr<Expr> right;
   Value accept(ExprVisitor &visitor) override {
    return visitor.visitLogicalExpr(*this);
    }

};
struct Unary : public Expr
{
    Unary ( Token my_operator, std::shared_ptr<Expr> right ) 
	: 	my_operator (my_operator),
	right (right){}
	 Token my_operator;	 std::shared_ptr<Expr> right;
   Value accept(ExprVisitor &visitor) override {
    return visitor.visitUnaryExpr(*this);
    }

};
struct Variable : public Expr
{
    Variable ( Token name ) 
	: 	name (name){}
	 Token name;
   Value accept(ExprVisitor &visitor) override {
    return visitor.visitVariableExpr(*this);
    }

};
