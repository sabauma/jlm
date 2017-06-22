/*
 * Copyright 2014 2015 Nico Reißmann <nico.reissmann@gmail.com>
 * See COPYING for terms of redistribution.
 */

#ifndef JLM_LLVM2JLM_TYPE_HPP
#define JLM_LLVM2JLM_TYPE_HPP

#include <jlm/ir/types.hpp>

#include <llvm/IR/Type.h>

#include <memory>

namespace jive {
namespace base {
	class type;
}
}

namespace llvm {
	class ArrayType;
	class Type;
}

namespace jlm {

class context;

std::unique_ptr<jive::value::type>
convert_type(const llvm::Type * type, context & ctx);

static inline std::unique_ptr<jlm::arraytype>
convert_arraytype(const llvm::ArrayType * type, context & ctx)
{
	auto t = convert_type(llvm::cast<llvm::Type>(type), ctx);
	JLM_DEBUG_ASSERT(is_arraytype(*t));
	return std::unique_ptr<jlm::arraytype>(static_cast<jlm::arraytype*>(t.release()));
}

}

#endif