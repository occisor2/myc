#pragma once

#include "analysis.h"
#include <memory>

// fixes recursive include
class NodeVisitor;

class NodeBase
{
public:
	virtual void accept(NodeVisitor& visitor) = 0;
};

class Expr : public NodeBase
{
public:
	enum class Type
	{
		NumLit,
		Bin,
	};

	Expr() = delete;
	Expr(Type type);
	virtual ~Expr() {}

	void accept(NodeVisitor& visitor) override;

	Type type;
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
