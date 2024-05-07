#pragma once

#include "analysis/visitor.h"
#include "token.h"
#include <memory>
#include <string>
#include <vector>

// fixes recursive include issue
class Visitor;

class NodeBase
{
public:
	NodeBase(Token token);
	~NodeBase() {}
	virtual void accept(Visitor& visitor) = 0;

	Token token;
};

class State : public NodeBase
{
public:
	enum class StateType
	{
		Compound,
		Expr,
		Decl,
		Return,
	};

	State() = delete;
	State(StateType stateType, Token token);
	virtual ~State() {}

	void accept(Visitor& visitor) override;

	StateType stateType;
};

class Expr : public State
{
public:
	enum class ExpType
	{
		NumLit,
		Ident,
		Bin,
	};

	Expr() = delete;
	Expr(ExpType expType, Token token);
	virtual ~Expr() {}

	void accept(Visitor& visitor) override;

	ExpType expType;
};

class Return : public State
{
public:
	Return() = delete;
	Return(std::unique_ptr<Expr> value, Token token);

	void accept(Visitor& visitor) override;

	std::unique_ptr<Expr> value;
};

class Ident : public Expr
{
public:
	Ident() = delete;
	Ident(std::string name, Token token);

	void accept(Visitor& visitor) override;

	std::string name;
};

class Decl : public State
{
public:
	Decl() = delete;
	Decl(std::unique_ptr<Ident> ident, std::unique_ptr<Expr> right, Token token);

	void accept(Visitor& visitor) override;

	std::unique_ptr<Ident> ident;
	std::unique_ptr<Expr> right;
};

class NumLit : public Expr
{
public:
	NumLit() = delete;
	NumLit(int numLit, Token token);

	void accept(Visitor& visitor);

	int numLit;
};

class BinExp : public Expr
{
public:
	enum class OpType
	{
		Sub,
		Add,
		Div,
		Mul
	};

	BinExp() = delete;
	BinExp(OpType opType, std::unique_ptr<Expr> left,
		   std::unique_ptr<Expr> right, Token token);

	void accept(Visitor& visitor);

	OpType opType;
	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
};

class Compound : public State
{
public:
	Compound() = delete;
	Compound(std::vector<std::unique_ptr<State>> statements, Token token);

	void accept(Visitor& visitor) override;

	std::vector<std::unique_ptr<State>> statements;
};

class FuncDecl : public NodeBase
{
public:
	FuncDecl() = delete;
	FuncDecl(std::unique_ptr<Ident> name, std::unique_ptr<Compound> body, Token token);
	virtual ~FuncDecl() {}

	void accept(Visitor& visitor) override;

	std::unique_ptr<Ident> name;
	std::unique_ptr<Compound> body;
};
