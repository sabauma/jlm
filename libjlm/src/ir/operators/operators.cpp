/*
 * Copyright 2014 2015 Nico Reißmann <nico.reissmann@gmail.com>
 * See COPYING for terms of redistribution.
 */

#include <jlm/ir/operators/operators.hpp>

#include <jive/arch/addresstype.h>
#include <jive/types/bitstring/constant.h>
#include <jive/types/float/flttype.h>

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/raw_ostream.h>

namespace jlm {

/* phi operator */

phi_op::~phi_op() noexcept
{}

bool
phi_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const phi_op*>(&other);
	return op && op->nodes_ == nodes_ && op->result(0) == result(0);
}

std::string
phi_op::debug_string() const
{
	std::string str("[");
	for (size_t n = 0; n < narguments(); n++) {
		str += strfmt(node(n));
		if (n != narguments()-1)
			str += ", ";
	}
	str += "]";

	return "PHI" + str;
}

std::unique_ptr<jive::operation>
phi_op::copy() const
{
	return std::unique_ptr<jive::operation>(new phi_op(*this));
}

/* assignment operator */

assignment_op::~assignment_op() noexcept
{}

bool
assignment_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const assignment_op*>(&other);
	return op && op->argument(0) == argument(0);
}

std::string
assignment_op::debug_string() const
{
	return "ASSIGN";
}

std::unique_ptr<jive::operation>
assignment_op::copy() const
{
	return std::unique_ptr<jive::operation>(new assignment_op(*this));
}

/* select operator */

select_op::~select_op() noexcept
{}

bool
select_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const select_op*>(&other);
	return op && op->result(0) == result(0);
}

std::string
select_op::debug_string() const
{
	return "SELECT";
}

std::unique_ptr<jive::operation>
select_op::copy() const
{
	return std::unique_ptr<jive::operation>(new select_op(*this));
}

/* fp2ui operator */

fp2ui_op::~fp2ui_op() noexcept
{}

bool
fp2ui_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const fp2ui_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->result(0) == result(0);
}

std::string
fp2ui_op::debug_string() const
{
	return "FP2UI";
}

std::unique_ptr<jive::operation>
fp2ui_op::copy() const
{
	return std::unique_ptr<jive::operation>(new fp2ui_op(*this));
}

jive_unop_reduction_path_t
fp2ui_op::can_reduce_operand(const jive::output * operand) const noexcept
{
	return jive_unop_reduction_none;
}

jive::output *
fp2ui_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	JLM_ASSERT(0);
}

/* fp2si operator */

fp2si_op::~fp2si_op() noexcept
{}

bool
fp2si_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const fp2si_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->result(0) == result(0);
}

std::string
fp2si_op::debug_string() const
{
	return "FP2UI";
}

std::unique_ptr<jive::operation>
fp2si_op::copy() const
{
	return std::unique_ptr<jive::operation>(new fp2si_op(*this));
}

jive_unop_reduction_path_t
fp2si_op::can_reduce_operand(const jive::output * operand) const noexcept
{
	return jive_unop_reduction_none;
}

jive::output *
fp2si_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	JLM_ASSERT(0);
}

/* ctl2bits operator */

ctl2bits_op::~ctl2bits_op() noexcept
{}

bool
ctl2bits_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const ctl2bits_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->result(0) == result(0);
}

std::string
ctl2bits_op::debug_string() const
{
	return "CTL2BITS";
}

std::unique_ptr<jive::operation>
ctl2bits_op::copy() const
{
	return std::unique_ptr<jive::operation>(new ctl2bits_op(*this));
}

/* branch operator */

branch_op::~branch_op() noexcept
{}

bool
branch_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const branch_op*>(&other);
	return op && op->argument(0) == argument(0);
}

std::string
branch_op::debug_string() const
{
	return "BRANCH";
}

std::unique_ptr<jive::operation>
branch_op::copy() const
{
	return std::unique_ptr<jive::operation>(new branch_op(*this));
}

/* ptr_constant_null operator */

ptr_constant_null_op::~ptr_constant_null_op() noexcept
{}

bool
ptr_constant_null_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const ptr_constant_null_op*>(&other);
	return op && op->result(0) == result(0);
}

std::string
ptr_constant_null_op::debug_string() const
{
	return "NULLPTR";
}

std::unique_ptr<jive::operation>
ptr_constant_null_op::copy() const
{
	return std::unique_ptr<jive::operation>(new ptr_constant_null_op(*this));
}

/* bits2ptr operator */

bits2ptr_op::~bits2ptr_op()
{}

bool
bits2ptr_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const jlm::bits2ptr_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->result(0) == result(0);
}

std::string
bits2ptr_op::debug_string() const
{
	return "BITS2PTR";
}

std::unique_ptr<jive::operation>
bits2ptr_op::copy() const
{
	return std::unique_ptr<jive::operation>(new jlm::bits2ptr_op(*this));
}

jive_unop_reduction_path_t
bits2ptr_op::can_reduce_operand(const jive::output * operand) const noexcept
{
	return jive_unop_reduction_none;
}

jive::output *
bits2ptr_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	JLM_ASSERT(0);
}

/* ptr2bits operator */

ptr2bits_op::~ptr2bits_op()
{}

bool
ptr2bits_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const jlm::ptr2bits_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->result(0) == result(0);
}

std::string
ptr2bits_op::debug_string() const
{
	return "PTR2BITS";
}

std::unique_ptr<jive::operation>
ptr2bits_op::copy() const
{
	return std::unique_ptr<jive::operation>(new jlm::ptr2bits_op(*this));
}

jive_unop_reduction_path_t
ptr2bits_op::can_reduce_operand(const jive::output * operand) const noexcept
{
	return jive_unop_reduction_none;
}

jive::output *
ptr2bits_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	JLM_ASSERT(0);
}

/* data array constant operator */

data_array_constant_op::~data_array_constant_op()
{}

bool
data_array_constant_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const data_array_constant_op*>(&other);
	return op && op->result(0) == result(0);
}

std::string
data_array_constant_op::debug_string() const
{
	return "ARRAYCONSTANT";
}

std::unique_ptr<jive::operation>
data_array_constant_op::copy() const
{
	return std::unique_ptr<jive::operation>(new data_array_constant_op(*this));
}

/* pointer compare operator */

ptrcmp_op::~ptrcmp_op()
{}

bool
ptrcmp_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const jlm::ptrcmp_op*>(&other);
	return op && op->argument(0) == argument(0) && op->cmp_ == cmp_;
}

std::string
ptrcmp_op::debug_string() const
{
	static std::unordered_map<jlm::cmp, std::string> map({
		{cmp::eq, "eq"}, {cmp::ne, "ne"}, {cmp::gt, "gt"}
	, {cmp::ge, "ge"}, {cmp::lt, "lt"}, {cmp::le, "le"}
	});

	JLM_DEBUG_ASSERT(map.find(cmp()) != map.end());
	return "PTRCMP " + map[cmp()];
}

std::unique_ptr<jive::operation>
ptrcmp_op::copy() const
{
	return std::unique_ptr<jive::operation>(new ptrcmp_op(*this));
}

jive_binop_reduction_path_t
ptrcmp_op::can_reduce_operand_pair(
	const jive::output * op1,
	const jive::output * op2) const noexcept
{
	return jive_binop_reduction_none;
}

jive::output *
ptrcmp_op::reduce_operand_pair(
	jive_binop_reduction_path_t path,
	jive::output * op1,
	jive::output * op2) const
{
	JLM_ASSERT(0);
}

/* zext operator */

zext_op::~zext_op()
{}

bool
zext_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const jlm::zext_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->result(0) == result(0);
}

std::string
zext_op::debug_string() const
{
	return strfmt("ZEXT[", nsrcbits(), " -> ", ndstbits(), "]");
}

std::unique_ptr<jive::operation>
zext_op::copy() const
{
	return std::unique_ptr<jive::operation>(new zext_op(*this));
}

jive_unop_reduction_path_t
zext_op::can_reduce_operand(const jive::output * operand) const noexcept
{
	if (jive::is<jive::bitconstant_op>(producer(operand)))
		return jive_unop_reduction_constant;

	return jive_unop_reduction_none;
}

jive::output *
zext_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	if (path == jive_unop_reduction_constant) {
		auto c = static_cast<const jive::bitconstant_op*>(&producer(operand)->operation());
		return create_bitconstant(operand->node()->region(), c->value().zext(ndstbits()-nsrcbits()));
	}

	return nullptr;
}

/* floating point constant operator */

fpconstant_op::~fpconstant_op()
{}

bool
fpconstant_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const jlm::fpconstant_op*>(&other);
	return op
	    && op->result(0) == result(0)
	    && op->constant().compare(constant()) == llvm::APFloatBase::cmpEqual;
}

std::string
fpconstant_op::debug_string() const
{
	llvm::SmallVector<char, 32> v;
	constant().toString(v, 32, 0);

	std::string s("FP(");
	for (const auto & c : v)
		s += c;
	s += ")";

	return s;
}

std::unique_ptr<jive::operation>
fpconstant_op::copy() const
{
	return std::unique_ptr<jive::operation>(new fpconstant_op(*this));
}

/* floating point comparison operator */

fpcmp_op::~fpcmp_op()
{}

bool
fpcmp_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const jlm::fpcmp_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->cmp_ == cmp_;
}

std::string
fpcmp_op::debug_string() const
{
	static std::unordered_map<fpcmp, std::string> map({
	  {fpcmp::oeq, "oeq"}, {fpcmp::ogt, "ogt"}, {fpcmp::oge, "oge"}, {fpcmp::olt, "olt"}
	, {fpcmp::ole, "ole"}, {fpcmp::one, "one"}, {fpcmp::ord, "ord"}, {fpcmp::ueq, "ueq"}
	, {fpcmp::ugt, "ugt"}, {fpcmp::uge, "uge"}, {fpcmp::ult, "ult"}, {fpcmp::ule, "ule"}
	, {fpcmp::une, "une"}, {fpcmp::uno, "uno"}
	});

	JLM_DEBUG_ASSERT(map.find(cmp()) != map.end());
	return "FPCMP " + map[cmp()];
}

std::unique_ptr<jive::operation>
fpcmp_op::copy() const
{
	return std::unique_ptr<jive::operation>(new jlm::fpcmp_op(*this));
}

jive_binop_reduction_path_t
fpcmp_op::can_reduce_operand_pair(
	const jive::output * op1,
	const jive::output * op2) const noexcept
{
	return jive_binop_reduction_none;
}

jive::output *
fpcmp_op::reduce_operand_pair(
	jive_binop_reduction_path_t path,
	jive::output * op1,
	jive::output * op2) const
{
	JLM_ASSERT(0);
}

/* undef constant operator */

undef_constant_op::~undef_constant_op()
{}

bool
undef_constant_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const jlm::undef_constant_op*>(&other);
	return op && op->result(0) == result(0);
}

std::string
undef_constant_op::debug_string() const
{
	return "undef";
}

std::unique_ptr<jive::operation>
undef_constant_op::copy() const
{
	return std::unique_ptr<jive::operation>(new jlm::undef_constant_op(*this));
}

/* floating point arithmetic operator */

fpbin_op::~fpbin_op()
{}

bool
fpbin_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const jlm::fpbin_op*>(&other);
	return op && op->fpop() == fpop() && op->size() == size();
}

std::string
fpbin_op::debug_string() const
{
	static std::unordered_map<jlm::fpop, std::string> map({
		{fpop::add, "add"}, {fpop::sub, "sub"}, {fpop::mul, "mul"}
	, {fpop::div, "div"}, {fpop::mod, "mod"}
	});

	JLM_DEBUG_ASSERT(map.find(fpop()) != map.end());
	return "FPOP " + map[fpop()];
}

std::unique_ptr<jive::operation>
fpbin_op::copy() const
{
	return std::unique_ptr<jive::operation>(new jlm::fpbin_op(*this));
}

jive_binop_reduction_path_t
fpbin_op::can_reduce_operand_pair(
	const jive::output * op1,
	const jive::output * op2) const noexcept
{
	return jive_binop_reduction_none;
}

jive::output *
fpbin_op::reduce_operand_pair(
	jive_binop_reduction_path_t path,
	jive::output * op1,
	jive::output * op2) const
{
	JLM_ASSERT(0);
}

/* fpext operator */

fpext_op::~fpext_op()
{}

bool
fpext_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const jlm::fpext_op*>(&other);
	return op && op->srcsize() == srcsize() && op->dstsize() == dstsize();
}

std::string
fpext_op::debug_string() const
{
	return "fpext";
}

std::unique_ptr<jive::operation>
fpext_op::copy() const
{
	return std::unique_ptr<jive::operation>(new jlm::fpext_op(*this));
}

jive_unop_reduction_path_t
fpext_op::can_reduce_operand(const jive::output * operand) const noexcept
{
	return jive_unop_reduction_none;
}

jive::output *
fpext_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	JLM_ASSERT(0);
}

/* fptrunc operator */

fptrunc_op::~fptrunc_op()
{}

bool
fptrunc_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const fptrunc_op*>(&other);
	return op && op->srcsize() == srcsize() && op->dstsize() == dstsize();
}

std::string
fptrunc_op::debug_string() const
{
	return "fptrunc";
}

std::unique_ptr<jive::operation>
fptrunc_op::copy() const
{
	return std::unique_ptr<jive::operation>(new fptrunc_op(*this));
}

jive_unop_reduction_path_t
fptrunc_op::can_reduce_operand(const jive::output * operand) const noexcept
{
	return jive_unop_reduction_none;
}

jive::output *
fptrunc_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	JLM_ASSERT(0);
}

/* valist operator */

valist_op::~valist_op()
{}

bool
valist_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const jlm::valist_op*>(&other);
	if (!op || op->narguments() != narguments())
		return false;

	for (size_t n = 0; n < narguments(); n++) {
		if (op->argument(n) != argument(n))
			return false;
	}

	return true;
}

std::string
valist_op::debug_string() const
{
	return "VALIST";
}

std::unique_ptr<jive::operation>
valist_op::copy() const
{
	return std::unique_ptr<jive::operation>(new jlm::valist_op(*this));
}

/* bitcast operator */

bitcast_op::~bitcast_op()
{}

bool
bitcast_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const bitcast_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->result(0) == result(0);
}

std::string
bitcast_op::debug_string() const
{
	return strfmt("BITCAST[", argument(0).type().debug_string(),
		" -> ", result(0).type().debug_string(), "]");
}

std::unique_ptr<jive::operation>
bitcast_op::copy() const
{
	return std::unique_ptr<jive::operation>(new bitcast_op(*this));
}

jive_unop_reduction_path_t
bitcast_op::can_reduce_operand(const jive::output * operand) const noexcept
{
	return jive_unop_reduction_none;
}

jive::output *
bitcast_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	JLM_ASSERT(0);
}

/* struct constant operator */

struct_constant_op::~struct_constant_op()
{}

bool
struct_constant_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const struct_constant_op*>(&other);
	return op && op->result(0) == result(0);
}

std::string
struct_constant_op::debug_string() const
{
	return "STRUCTCONSTANT";
}

std::unique_ptr<jive::operation>
struct_constant_op::copy() const
{
	return std::unique_ptr<jive::operation>(new struct_constant_op(*this));
}

/* trunc operator */

trunc_op::~trunc_op()
{}

bool
trunc_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const trunc_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->result(0) == result(0);
}

std::string
trunc_op::debug_string() const
{
	return strfmt("TRUNC[", nsrcbits(), " -> ", ndstbits(), "]");
}

std::unique_ptr<jive::operation>
trunc_op::copy() const
{
	return std::unique_ptr<jive::operation>(new trunc_op(*this));
}

jive_unop_reduction_path_t
trunc_op::can_reduce_operand(const jive::output * operand) const noexcept
{
	return jive_unop_reduction_none;
}

jive::output *
trunc_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	JLM_ASSERT(0);
}


/* uitofp operator */

uitofp_op::~uitofp_op()
{}

bool
uitofp_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const uitofp_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->result(0) == result(0);
}

std::string
uitofp_op::debug_string() const
{
	return "UITOFP";
}

std::unique_ptr<jive::operation>
uitofp_op::copy() const
{
	return std::make_unique<uitofp_op>(*this);
}

jive_unop_reduction_path_t
uitofp_op::can_reduce_operand(
	const jive::output * operand) const noexcept
{
	return jive_unop_reduction_none;
}

jive::output *
uitofp_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	JLM_ASSERT(0);
}

/* sitofp operator */

sitofp_op::~sitofp_op()
{}

bool
sitofp_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const sitofp_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->result(0) == result(0);
}

std::string
sitofp_op::debug_string() const
{
	return "SITOFP";
}

std::unique_ptr<jive::operation>
sitofp_op::copy() const
{
	return std::make_unique<sitofp_op>(*this);
}

jive_unop_reduction_path_t
sitofp_op::can_reduce_operand(const jive::output * operand) const noexcept
{
	return jive_unop_reduction_none;
}

jive::output *
sitofp_op::reduce_operand(
	jive_unop_reduction_path_t path,
	jive::output * operand) const
{
	JLM_ASSERT(0);
}

/* constant array operator */

constant_array_op::~constant_array_op()
{}

bool
constant_array_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const constant_array_op*>(&other);
	return op && op->result(0) == result(0);
}

std::string
constant_array_op::debug_string() const
{
	return "CONSTANTARRAY";
}

std::unique_ptr<jive::operation>
constant_array_op::copy() const
{
	return std::unique_ptr<jive::operation>(new constant_array_op(*this));
}

/* constant aggregate zero operator */

constant_aggregate_zero_op::~constant_aggregate_zero_op()
{}

bool
constant_aggregate_zero_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const constant_aggregate_zero_op*>(&other);
	return op && op->result(0) == result(0);
}

std::string
constant_aggregate_zero_op::debug_string() const
{
	return "CONSTANT_AGGREGATE_ZERO";
}

std::unique_ptr<jive::operation>
constant_aggregate_zero_op::copy() const
{
	return std::unique_ptr<jive::operation>(new constant_aggregate_zero_op(*this));
}

/* extractelement operator */

extractelement_op::~extractelement_op()
{}

bool
extractelement_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const extractelement_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->argument(1) == argument(1);
}

std::string
extractelement_op::debug_string() const
{
	return "EXTRACTELEMENT";
}

std::unique_ptr<jive::operation>
extractelement_op::copy() const
{
	return std::unique_ptr<jive::operation>(new extractelement_op(*this));
}

/* shufflevector operator */

shufflevector_op::~shufflevector_op()
{}

bool
shufflevector_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const shufflevector_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->argument(2) == argument(2);
}

std::string
shufflevector_op::debug_string() const
{
	return "SHUFFLEVECTOR";
}

std::unique_ptr<jive::operation>
shufflevector_op::copy() const
{
	return std::unique_ptr<jive::operation>(new shufflevector_op(*this));
}

/* constantvector operator */

constantvector_op::~constantvector_op()
{}

bool
constantvector_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const constantvector_op*>(&other);
	return op
	    && op->result(0) == result(0);
}

std::string
constantvector_op::debug_string() const
{
	return "CONSTANTVECTOR";
}

std::unique_ptr<jive::operation>
constantvector_op::copy() const
{
	return std::unique_ptr<jive::operation>(new constantvector_op(*this));
}

/* insertelement operator */

insertelement_op::~insertelement_op()
{}

bool
insertelement_op::operator==(const operation & other) const noexcept
{
	auto op = dynamic_cast<const insertelement_op*>(&other);
	return op
	    && op->argument(0) == argument(0)
	    && op->argument(1) == argument(1)
	    && op->argument(2) == argument(2);
}

std::string
insertelement_op::debug_string() const
{
	return "INSERTELEMENT";
}

std::unique_ptr<jive::operation>
insertelement_op::copy() const
{
	return std::unique_ptr<jive::operation>(new insertelement_op(*this));
}

/* vectorunary operator */

vectorunary_op::~vectorunary_op()
{}

bool
vectorunary_op::operator==(const jive::operation & other) const noexcept
{
	auto op = dynamic_cast<const vectorunary_op*>(&other);
	return op && op->operation() == operation();
}

std::string
vectorunary_op::debug_string() const
{
	return strfmt("VEC", operation().debug_string());
}

std::unique_ptr<jive::operation>
vectorunary_op::copy() const
{
	return std::unique_ptr<jive::operation>(new vectorunary_op(*this));
}

/* vectorbinary operator */

vectorbinary_op::~vectorbinary_op()
{}

bool
vectorbinary_op::operator==(const jive::operation & other) const noexcept
{
	auto op = dynamic_cast<const vectorbinary_op*>(&other);
	return op && op->operation() == operation();
}

std::string
vectorbinary_op::debug_string() const
{
	return strfmt("VEC", operation().debug_string());
}

std::unique_ptr<jive::operation>
vectorbinary_op::copy() const
{
	return std::unique_ptr<jive::operation>(new vectorbinary_op(*this));
}

/* const data vector operator */

constant_data_vector_op::~constant_data_vector_op()
{}

bool
constant_data_vector_op::operator==(const jive::operation & other) const noexcept
{
	auto op = dynamic_cast<const constant_data_vector_op*>(&other);
	return op && op->result(0) == result(0);
}

std::string
constant_data_vector_op::debug_string() const
{
	return "CONSTANTDATAVECTOR";
}

std::unique_ptr<jive::operation>
constant_data_vector_op::copy() const
{
	return std::unique_ptr<jive::operation>(new constant_data_vector_op(*this));
}

/* extractvalue operator */

extractvalue_op::~extractvalue_op()
{}

bool
extractvalue_op::operator==(const jive::operation & other) const noexcept
{
	auto op = dynamic_cast<const extractvalue_op*>(&other);
	return op
	    && op->indices_ == indices_
	    && op->argument(0) == argument(0);
}

std::string
extractvalue_op::debug_string() const
{
	return "EXTRACTVALUE";
}

std::unique_ptr<jive::operation>
extractvalue_op::copy() const
{
	return std::unique_ptr<jive::operation>(new extractvalue_op(*this));
}

/* loop state mux operator */

loopstatemux_op::~loopstatemux_op()
{}

bool
loopstatemux_op::operator==(const jive::operation & other) const noexcept
{
	return dynamic_cast<const loopstatemux_op*>(&other) != nullptr;
}

std::string
loopstatemux_op::debug_string() const
{
	return "LOOPSTATEMUX";
}

std::unique_ptr<jive::operation>
loopstatemux_op::copy() const
{
	return std::unique_ptr<jive::operation>(new loopstatemux_op(*this));
}

}
