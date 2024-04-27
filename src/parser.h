#pragma once

#include "ast/ast.h"
#include "ast/expr.h"
#include "scanner.h"
#include "token.h"
#include <memory>
#include <string_view>

class Parser
{
public:
	Parser() = delete;
	Parser(std::string_view sourceCode);

	AST parse();
	
private:
	std::string_view sourceCode;
	Scanner scanner;
	Token prev;
	Token current;

	void error(std::string_view msg);
	std::string getTokenText(Token token);

	void next();
	void consume(Token::Type type, std::string_view errmsg);

	// requires ordering for parser
	enum class Precedence : int
	{
		None,
		Assignment,  // =
		Or,          // or
		And,         // and
		Equality,    // == !=
		Comparison,  // < > <= >=
		Term,        // + -
		Factor,      // * /
		Unary,       // ! -
		Call,        // . ()
		Primary
	};

	using ParseFn = std::unique_ptr<Expr>(Parser::*)();
	struct RuleEntry
	{
		ParseFn prefix;
		ParseFn infix;
		Precedence prec;
	};

	static constexpr RuleEntry table[] = {
		{nullptr, nullptr, Precedence::None}
	};

	RuleEntry getRuleEntry(Token::Type type);
	std::unique_ptr<Expr> parseExpression(Precedence prec);
};
