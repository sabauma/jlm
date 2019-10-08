/*
 * Copyright 2015 Nico Reißmann <nico.reissmann@gmail.com>
 * See COPYING for terms of redistribution.
 */

#ifndef JLM_LLVM2JLM_CONTEXT_HPP
#define JLM_LLVM2JLM_CONTEXT_HPP

#include <jlm/ir/cfg-node.hpp>
#include <jlm/ir/module.hpp>
#include <jlm/ir/tac.hpp>
#include <jlm/llvm2jlm/type.hpp>

#include <jive/types/record.h>
#include <llvm/IR/DerivedTypes.h>

#include <unordered_map>

namespace llvm {
	class BasicBlock;
	class Function;
	class Value;
}

namespace jlm {

class cfg;
class cfg_node;
class clg_node;
class module;
class variable;

class basic_block_map final {
public:
	inline bool
	has(const llvm::BasicBlock * bb) const noexcept
	{
		return llvm2jlm_.find(bb) != llvm2jlm_.end();
	}

	inline bool
	has(const basic_block * bb) const noexcept
	{
		return jlm2llvm_.find(bb) != jlm2llvm_.end();
	}

	inline basic_block *
	get(const llvm::BasicBlock * bb) const noexcept
	{
		JLM_DEBUG_ASSERT(has(bb));
		return llvm2jlm_.find(bb)->second;
	}

	inline const llvm::BasicBlock *
	get(const basic_block * bb) const noexcept
	{
		JLM_DEBUG_ASSERT(has(bb));
		return jlm2llvm_.find(bb)->second;
	}

	inline void
	insert(const llvm::BasicBlock * bb1, basic_block * bb2)
	{
		JLM_DEBUG_ASSERT(!has(bb1));
		JLM_DEBUG_ASSERT(!has(bb2));
		llvm2jlm_[bb1] = bb2;
		jlm2llvm_[bb2] = bb1;
	}

	basic_block *
	operator[](const llvm::BasicBlock * bb) const
	{
		return get(bb);
	}

	const llvm::BasicBlock *
	operator[](const basic_block * bb) const
	{
		return get(bb);
	}

private:
	std::unordered_map<const llvm::BasicBlock*, basic_block*> llvm2jlm_;
	std::unordered_map<const basic_block*, const llvm::BasicBlock*> jlm2llvm_;
};

class context final {
public:
	inline
	context(jlm::module & module)
		: module_(module)
		, node_(nullptr)
	{}

	inline jlm::variable *
	result() const noexcept
	{
		return result_;
	}

	inline void
	set_result(jlm::variable * result)
	{
		result_ = result;
	}

	inline jlm::variable *
	state() const noexcept
	{
		return state_;
	}

	inline void
	set_state(jlm::variable * state)
	{
		state_ = state;
	}

	inline bool
	has(const llvm::BasicBlock * bb) const noexcept
	{
		return bbmap_.has(bb);
	}

	inline bool
	has(const basic_block * bb) const noexcept
	{
		return bbmap_.has(bb);
	}

	inline basic_block *
	get(const llvm::BasicBlock * bb) const noexcept
	{
		return bbmap_.get(bb);
	}

	inline const llvm::BasicBlock *
	get(const basic_block * bb) const noexcept
	{
		return bbmap_.get(bb);
	}

	inline void
	set_basic_block_map(const basic_block_map & bbmap)
	{
		bbmap_ = bbmap;
	}

	inline bool
	has_value(const llvm::Value * value) const noexcept
	{
		return vmap_.find(value) != vmap_.end();
	}

	inline const jlm::variable *
	lookup_value(const llvm::Value * value) const noexcept
	{
		JLM_DEBUG_ASSERT(has_value(value));
		return vmap_.find(value)->second;
	}

	inline void
	insert_value(const llvm::Value * value, const jlm::variable * variable)
	{
		JLM_DEBUG_ASSERT(!has_value(value));
		vmap_[value] = variable;
	}

	inline const jive::rcddeclaration *
	lookup_declaration(const llvm::StructType * type)
	{
		/* FIXME: They live as long as jlm is alive. */
		static std::vector<std::unique_ptr<jive::rcddeclaration>> dcls;

		auto it = declarations_.find(type);
		if (it != declarations_.end())
			return it->second;

		auto dcl = jive::rcddeclaration::create();
		declarations_[type] = dcl.get();
		for (size_t n = 0; n < type->getNumElements(); n++)
			dcl->append(*convert_type(type->getElementType(n), *this));

		dcls.push_back(std::move(dcl));
		return declarations_[type];
	}

	inline jlm::module &
	module() const noexcept
	{
		return module_;
	}

	inline void
	set_node(ipgraph_node * node) noexcept
	{
		node_ = node;
	}

	inline ipgraph_node *
	node() const noexcept
	{
		return node_;
	}

private:
	jlm::module & module_;
	basic_block_map bbmap_;
	jlm::variable * state_;
	ipgraph_node * node_;
	jlm::variable * result_;
	std::unordered_map<const llvm::Value*, const jlm::variable*> vmap_;
	std::unordered_map<
		const llvm::StructType*,
		const jive::rcddeclaration*> declarations_;
};

}

#endif
