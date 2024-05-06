#pragma once

#include "ast.h"
#include "diagnostic.h"
#include "scanner.h"
#include "token.h"
#include <memory>
#include <string_view>

class SyntaxError : public Diagnostic
{
public:
	SyntaxError(Token badToken, std::string fileName, std::string msg)
		: Diagnostic(Diagnostic::Type::Error, badToken, fileName, msg) {}
};

class Parser
{
public:
	Parser() = delete;
	Parser(std::string_view sourceCode, std::string fileName);

	void parse();
	
private:
	std::string fileName;
	std::string_view sourceCode;
	Scanner scanner;
	Token prev;
	Token current;

	void error(const std::string& msg, Token badToken);
	std::string getTokenText(Token token);

	Token next();
	void consume(Token::Type type, const std::string& errmsg);

	std::unique_ptr<State> parseStatement();
	std::unique_ptr<Ident> parseIdent();
	std::unique_ptr<Decl> declaration();

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

	std::unique_ptr<Expr> parseExpression();
	std::unique_ptr<Expr> parsePrecedence(Precedence precedence);
	std::unique_ptr<Expr> number();
	std::unique_ptr<Expr> identifier();
	std::unique_ptr<Expr> grouping();
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
		{nullptr,          nullptr,         Precedence::None},    // eof
		{&Parser::number,  nullptr,         Precedence::Primary}, // num
		{nullptr,          &Parser::binary, Precedence::Term},    // sub
		{nullptr,          &Parser::binary, Precedence::Term},    // add
		{nullptr,          &Parser::binary, Precedence::Factor},  // div
		{nullptr,          &Parser::binary, Precedence::Factor},  // mul
		{&Parser::grouping,nullptr,         Precedence::None},    // (
		{nullptr,          nullptr,         Precedence::None},    // )
		{nullptr,          nullptr,         Precedence::None},    // ;
		{nullptr,          nullptr,         Precedence::None},    // =
		{nullptr,         nullptr,         Precedence::None},    // {
		{nullptr ,        nullptr,         Precedence::None},    // }
		{&Parser::identifier,nullptr,      Precedence::Primary}, // ident
		{nullptr,         nullptr,         Precedence::None},    // error
		{nullptr,         nullptr,         Precedence::None},    // int
	};

	Rule getRule(Token::Type type) const;
};
