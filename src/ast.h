#pragma once

#include "analysis.h"
#include <memory>
#include <string>

// fixes recursive include
class NodeVisitor;

class NodeBase
{
public:
	virtual void accept(NodeVisitor& visitor) = 0;
};

class State : public NodeBase
{
public:
	enum class StateType
	{
		Expr,
		Decl,
	};

	State() = delete;
	State(StateType stateType);
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
	Expr(ExpType expType);
	virtual ~Expr() {}

	void accept(NodeVisitor& visitor) override;

	ExpType expType;
};

class Ident : public Expr
{
public:
	Ident() = delete;
	Ident(std::string name);

	void accept(NodeVisitor& visitor) override;

	std::string name;
};

class Decl : public State
{
public:
	Decl() = delete;
	Decl(std::unique_ptr<Ident> ident, std::unique_ptr<Expr> right);

	void accept(NodeVisitor& visitor) override;

	std::unique_ptr<Ident> ident;
	std::unique_ptr<Expr> right;
};

class NumLit : public Expr
{
public:
	NumLit() = delete;
	NumLit(int numLit);

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
	BinExp(OpType opType, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right);

	void accept(NodeVisitor& visitor);

	OpType opType;
	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;
};
