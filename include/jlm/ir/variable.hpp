/*
 * Copyright 2015 Nico Reißmann <nico.reissmann@gmail.com>
 * See COPYING for terms of redistribution.
 */

#ifndef JLM_IR_VARIABLE_HPP
#define JLM_IR_VARIABLE_HPP

#include <jive/vsdg/basetype.h>

#include <jlm/util/strfmt.hpp>

#include <memory>
#include <sstream>

namespace jlm {

/* variable */

class variable {
public:
	virtual
	~variable() noexcept;

	inline
	variable(const jive::base::type & type, const std::string & name, bool exported)
		: exported_(exported)
		, name_(name)
		, type_(type.copy())
	{}

	virtual std::string
	debug_string() const;

	inline const std::string &
	name() const noexcept
	{
		return name_;
	}

	inline bool
	exported() const noexcept
	{
		return exported_;
	}

	inline const jive::base::type &
	type() const noexcept
	{
		return *type_;
	}

private:
	bool exported_;
	std::string name_;
	std::unique_ptr<jive::base::type> type_;
};

/* top level variable */

enum class linkage {
	external_linkage, available_externally_linkage, link_once_any_linkage,
	link_once_odr_linkage, weak_any_linkage, weak_odr_linkage,
	appending_linkage, internal_linkage, private_linkage,
	external_weak_linkage, common_linkage
};

class gblvariable : public variable {
public:
	virtual
	~gblvariable();

	inline
	gblvariable(
		const jive::base::type & type,
		const std::string & name,
		const jlm::linkage & linkage)
	: variable(type, name, linkage != linkage::internal_linkage)
	, linkage_(linkage)
	{}

	inline const jlm::linkage &
	linkage() const noexcept
	{
		return linkage_;
	}

private:
	jlm::linkage linkage_;
};

}

#endif
