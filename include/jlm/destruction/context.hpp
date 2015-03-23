/*
 * Copyright 2015 Nico Reißmann <nico.reissmann@gmail.com>
 * See COPYING for terms of redistribution.
 */

#ifndef JLM_DESTRUCTION_CONTEXT_HPP
#define JLM_DESTRUCTION_CONTEXT_HPP

#include <jlm/common.hpp>
#include <jlm/IR/cfg_node.hpp>
#include <jlm/IR/variable.hpp>

#include <jive/vsdg/node.h>
#include <jive/vsdg/theta.h>

#include <stack>
#include <unordered_map>

namespace jlm {
namespace frontend {
	class clg_node;
}

namespace dstrct {

class variable_map final {
public:

	inline bool
	has_value(const jlm::frontend::variable * v) const noexcept
	{
		return map_.find(v) != map_.end();
	}

	inline jive::output *
	lookup_value(const jlm::frontend::variable * v) const
	{
		JLM_DEBUG_ASSERT(has_value(v));
		return map_.find(v)->second;
	}

	inline void
	insert_value(const jlm::frontend::variable * v, jive::output * o)
	{
		map_[v] = o;
	}

	inline void
	replace_value(const jlm::frontend::variable * v, jive::output * o)
	{
		JLM_DEBUG_ASSERT(has_value(v));
		map_[v] = o;
	}

	inline void
	merge(const variable_map & vmap)
	{
		map_.insert(vmap.map_.begin(), vmap.map_.end());
	}

	inline size_t
	size() const noexcept
	{
		return map_.size();
	}

	//FIXME: this is an ugly solution
	std::unordered_map<const jlm::frontend::variable*, jive::output*>::iterator
	begin()
	{
		return map_.begin();
	}

	std::unordered_map<const jlm::frontend::variable*, jive::output*>::iterator
	end()
	{
		return map_.end();
	}

private:
	std::unordered_map<const jlm::frontend::variable*, jive::output*> map_;
};

class predicate_stack final {
public:
	inline size_t
	size() const noexcept
	{
		return stack_.size();
	}

	inline bool
	empty() const noexcept
	{
		return size() == 0;
	}

	inline std::pair<jive::output*,size_t>
	top() const noexcept
	{
		JLM_DEBUG_ASSERT(!empty());
		return stack_.top();
	}

	inline void
	push(jive::output * o, size_t index)
	{
		stack_.push(std::make_pair(o, index));
	}

	inline void
	pop()
	{
		if (!empty())
			stack_.pop();
	}

	inline std::pair<jive::output*,size_t>
	poptop()
	{
		std::pair<jive::output*,size_t> p = top();
		pop();
		return p;
	}

private:
	std::stack<std::pair<jive::output*, size_t>> stack_;
};

class theta_env final {
public:
	inline
	theta_env(struct jive_region * region)
		: theta_(jive_theta_begin(region))
	{}

	inline jive_theta *
	theta()
	{
		return &theta_;
	}

	inline bool
	has_loopvar(const jlm::frontend::variable * v) const
	{
		return loopvars_.find(v) != loopvars_.end();
	}

	inline jive_theta_loopvar
	lookup_loopvar(const jlm::frontend::variable * v)
	{
		JLM_DEBUG_ASSERT(has_loopvar(v));
		return loopvars_.find(v)->second;
	}

	inline void
	insert_loopvar(const jlm::frontend::variable * v, jive_theta_loopvar lv)
	{
		JLM_DEBUG_ASSERT(!has_loopvar(v));
		loopvars_[v] = lv;
	}

	inline void
	replace_loopvar(const frontend::variable * v, jive_theta_loopvar lv)
	{
		JLM_DEBUG_ASSERT(has_loopvar(v));
		loopvars_[v] = lv;
	}

	/* FIXME: this is a really ugly hack */
	std::unordered_map<const jlm::frontend::variable*, jive_theta_loopvar>::iterator
	begin()
	{
		return loopvars_.begin();
	}

	std::unordered_map<const jlm::frontend::variable*, jive_theta_loopvar>::iterator
	end()
	{
		return loopvars_.end();
	}

private:
	jive_theta theta_;
	std::unordered_map<const jlm::frontend::variable *, jive_theta_loopvar> loopvars_;
};

class theta_stack final {
public:
	inline size_t
	size() const noexcept
	{
		return stack_.size();
	}

	inline bool
	empty() const noexcept
	{
		return size() == 0;
	}

	inline theta_env *
	top()
	{
		JLM_DEBUG_ASSERT(!empty());
		return stack_.top();
	}

	inline void
	push(theta_env * t)
	{
		stack_.push(t);
	}

	inline void
	pop()
	{
		if (!empty())
			stack_.pop();
	}

	inline theta_env *
	poptop()
	{
		theta_env * t = top();
		pop();
		return t;
	}

private:
	std::stack<theta_env*> stack_;
};

class context final {
public:
	inline bool
	has_variable_map(const jlm::frontend::cfg_node * node) const noexcept
	{
		return vmap_.find(node) != vmap_.end();
	}

	inline variable_map &
	lookup_variable_map(const jlm::frontend::cfg_node * node)
	{
		JLM_DEBUG_ASSERT(has_variable_map(node));
		return vmap_.find(node)->second;
	}

	inline void
	insert_variable_map(const jlm::frontend::cfg_node * node, const variable_map & vmap)
	{
		JLM_DEBUG_ASSERT(!has_variable_map(node));
		vmap_[node] = vmap;
	}

	inline bool
	has_predicate_stack(const jlm::frontend::cfg_edge * edge) const noexcept
	{
		return pmap_.find(edge) != pmap_.end();
	}

	inline predicate_stack &
	lookup_predicate_stack(const jlm::frontend::cfg_edge * edge)
	{
		JLM_DEBUG_ASSERT(has_predicate_stack(edge));
		return pmap_.find(edge)->second;
	}

	inline void
	insert_predicate_stack(const jlm::frontend::cfg_edge * edge, const predicate_stack & pstack)
	{
		JLM_DEBUG_ASSERT(!has_predicate_stack(edge));
		pmap_[edge] = pstack;
	}

	inline bool
	has_theta_stack(const jlm::frontend::cfg_edge * edge) const noexcept
	{
		return tmap_.find(edge) != tmap_.end();
	}

	inline theta_stack &
	lookup_theta_stack(const jlm::frontend::cfg_edge * edge)
	{
		JLM_DEBUG_ASSERT(has_theta_stack(edge));
		return tmap_.find(edge)->second;
	}

	inline void
	insert_theta_stack(const jlm::frontend::cfg_edge * edge, const theta_stack & tstack)
	{
		JLM_DEBUG_ASSERT(!has_theta_stack(edge));
		tmap_[edge] = tstack;
	}

	theta_env *
	create_theta_env(struct jive_region * region)
	{
		std::unique_ptr<theta_env> tenv(new theta_env(region));
		theta_env * env = tenv.get();
		theta_envs_.insert(std::move(tenv));
		tenv.release();
		return env;
	}

	inline bool
	has_function(const frontend::clg_node * function) const noexcept
	{
		return fmap_.find(function) != fmap_.end();
	}

	inline jive::output *
	lookup_function(const frontend::clg_node * function) const noexcept
	{
		JLM_DEBUG_ASSERT(has_function(function));
		return fmap_.find(function)->second;
	}

	inline void
	insert_function(const frontend::clg_node * f, jive::output * r)
	{
		JIVE_DEBUG_ASSERT(!has_function(f));
		fmap_[f] = r;
	}

	inline void
	replace_function(const frontend::clg_node * f, jive::output * r)
	{
		JIVE_DEBUG_ASSERT(has_function(f));
		fmap_[f] = r;
	}

private:
	std::unordered_set<std::unique_ptr<theta_env>> theta_envs_;
	std::unordered_map<const jlm::frontend::clg_node*, jive::output*> fmap_;
	std::unordered_map<const jlm::frontend::cfg_node*, variable_map> vmap_;
	std::unordered_map<const jlm::frontend::cfg_edge*, predicate_stack> pmap_;
	std::unordered_map<const jlm::frontend::cfg_edge*, theta_stack> tmap_;
};

}
}

#endif