#include "Instruct.h"

IR::Instruct::Instruct(Operator op, IR::Addr arg1, IR::Addr arg2, IR::Addr result)
	: op(op), arg1(arg1), arg2(arg2), result(result)
{}

std::ostream& IR::operator<<(std::ostream& os, const IR::Instruct& t)
{
	auto text = t.opTextTable.at(t.op);
	os << t.result << " = " << t.arg1 << " " << text << " " << t.arg2;
	return os;
}
