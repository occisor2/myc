#pragma once

#include "analysis.h"
#include "token.h"
#include <memory>
#include <string>
#include <vector>

// fixes recursive include
class NodeVisitor;

class NodeBase
{
public:
	NodeBase(Token token);
	~NodeBase() {}
	virtual void accept(NodeVisitor& visitor) = 0;

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
	};

	State() = delete;
	State(StateType stateType, Token token);
	virtual ~State() {}

	void accept(NodeVisitor& visitor) override;

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

	void accept(NodeVisitor& visitor) override;

	ExpType expType;
};

class Ident : public Expr
{
public:
	Ident() = delete;
	Ident(std::string name, Token token);

	void accept(NodeVisitor& visitor) override;

	std::string name;
};

class Decl : public State
{
public:
	Decl() = delete;
	Decl(std::unique_ptr<Ident> ident, std::unique_ptr<Expr> right, Token token);

	void accept(NodeVisitor& visitor) override;

	std::unique_ptr<Ident> ident;
	std::unique_ptr<Expr> right;
};

class NumLit : public Expr
{
public:
	NumLit() = delete;
	NumLit(int numLit, Token token);

	void accept(NodeVisitor& visitor);

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

	void accept(NodeVisitor& visitor);

	OpType opType;
	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
};

class Compound : public State
{
public:
	Compound() = delete;
	Compound(std::vector<std::unique_ptr<State>> statements, Token token);

	void accept(NodeVisitor& visitor) override;

	std::vector<std::unique_ptr<State>> statements;
};

class FuncDecl : NodeBase
{
public:
	FuncDecl() = delete;
	FuncDecl(std::unique_ptr<Ident> name, std::unique_ptr<Compound> body, Token token);

	void accept(NodeVisitor& visitor) override;

	std::unique_ptr<Ident> name;
	std::unique_ptr<Compound> body;
};
