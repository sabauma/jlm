/*
 * Copyright 2015 Nico Reißmann <nico.reissmann@gmail.com>
 * See COPYING for terms of redistribution.
 */

#ifndef JLM_IR_VARIABLE_HPP
#define JLM_IR_VARIABLE_HPP

#include <jive/rvsdg/type.h>

#include <jlm/ir/linkage.hpp>
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
	variable(const jive::type & type, const std::string & name)
	: name_(name)
	, type_(type.copy())
	{}

	virtual std::string
	debug_string() const;

	inline const std::string &
	name() const noexcept
	{
		return name_;
	}

	inline const jive::type &
	type() const noexcept
	{
		return *type_;
	}

private:
	std::string name_;
	std::unique_ptr<jive::type> type_;
};

template <class T> static inline bool
is(const jlm::variable * variable) noexcept
{
	static_assert(std::is_base_of<jlm::variable, T>::value,
		"Template parameter T must be derived from jlm::variable.");

	return dynamic_cast<const T*>(variable) != nullptr;
}

/* top level variable */

class gblvariable : public variable {
public:
	virtual
	~gblvariable();

	inline
	gblvariable(
		const jive::type & type,
		const std::string & name)
	: variable(type, name)
	{}
};

}

#endif
