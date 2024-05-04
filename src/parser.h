#pragma once

#include "ast.h"
#include "scanner.h"
#include "token.h"
#include <memory>
#include <string_view>

class Parser
{
public:
	Parser() = delete;
	Parser(std::string_view sourceCode);

	void parse();
	
private:
	std::string_view sourceCode;
	Scanner scanner;
	Token prev;
	Token current;

	void error(std::string_view msg);
	std::string getTokenText(Token token);

	Token next();
	void consume(Token::Type type, std::string_view errmsg);

	/*
	 * All methods and data below mostly belong to the Pratt Parser
	 * implementation.
	 */
	enum class Precedence : int
	{
		// requires ordering for parser
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

	// If these functions get called, there is a programmer error in the Pratt
	// parser.
	std::unique_ptr<Expr> badToken();
	std::unique_ptr<Expr> badTokenBin(std::unique_ptr<Expr>);

	std::unique_ptr<Expr> parseExpression();
	std::unique_ptr<Expr> parsePrecedence(Precedence precedence);
	std::unique_ptr<Expr> primary();
	std::unique_ptr<Expr> binary(std::unique_ptr<Expr> left);

	using PrefixFn = std::unique_ptr<Expr>(Parser::*)();
	using InfixFn = std::unique_ptr<Expr>(Parser::*)(std::unique_ptr<Expr>);
	struct Rule
	{
		PrefixFn prefix;
		InfixFn infix;
		Precedence prec;
	};

	static constexpr Rule table[] = {
		{nullptr,            nullptr,              Precedence::None},    // eof
		{&Parser::primary,   nullptr,              Precedence::Primary}, // num
		{nullptr,            &Parser::binary,      Precedence::Term},    // sub
		{nullptr,            &Parser::binary,      Precedence::Term},    // add
		{nullptr,            &Parser::binary,      Precedence::Factor},  // div
		{nullptr,            &Parser::binary,      Precedence::Factor},  // mul
		{&Parser::badToken,  &Parser::badTokenBin, Precedence::None},    // (
		{&Parser::badToken,  &Parser::badTokenBin, Precedence::None},    // )
		{&Parser::badToken,  &Parser::badTokenBin, Precedence::None},    // ;
		{&Parser::badToken,  &Parser::badTokenBin, Precedence::None},    // =
		{&Parser::badToken, &Parser::badTokenBin, Precedence::None},    // {
		{&Parser::badToken, &Parser::badTokenBin, Precedence::None},    // }
		{&Parser::badToken, &Parser::badTokenBin, Precedence::None},    // ident
		{&Parser::badToken, &Parser::badTokenBin, Precedence::None},    // error
		{&Parser::badToken, &Parser::badTokenBin, Precedence::None},    // int
	};

	Rule getRule(Token::Type type) const;
};
