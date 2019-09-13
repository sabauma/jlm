/*
 * Copyright 2017 Nico Reißmann <nico.reissmann@gmail.com>
 * See COPYING for terms of redistribution.
 */

#include <jive/view.h>

#include <jlm/ir/module.hpp>
#include <jlm/ir/operators.hpp>
#include <jlm/ir/rvsdg.hpp>
#include <jlm/jlm2rvsdg/module.hpp>
#include <jlm/jlm2llvm/jlm2llvm.hpp>
#include <jlm/llvm2jlm/module.hpp>
#include <jlm/opt/optimization.hpp>
#include <jlm/rvsdg2jlm/rvsdg2jlm.hpp>

#include <jlm-opt/cmdline.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/SourceMgr.h>

#include <iostream>

static std::unique_ptr<llvm::Module>
parse_llvm_file(
	const char * executable,
	const jlm::filepath & file,
	llvm::LLVMContext & ctx)
{
	llvm::SMDiagnostic d;
	auto module = llvm::parseIRFile(file.to_str(), d, ctx);
	if (!module) {
		d.print(executable, llvm::errs());
		exit(EXIT_FAILURE);
	}

	return module;
}

static std::unique_ptr<jlm::module>
construct_jlm_module(llvm::Module & module)
{
	return jlm::convert_module(module);
}

static void
print_as_xml(const jlm::rvsdg & rvsdg, const jlm::filepath & fp)
{
	auto fd = fp == "" ? stdout : fopen(fp.to_str().c_str(), "w");

	jive::view_xml(rvsdg.graph()->root(), fd);

	if (fd != stdout)
			fclose(fd);
}

static void
print_as_llvm(const jlm::rvsdg & rvsdg, const jlm::filepath & fp)
{
	auto jlm_module = jlm::rvsdg2jlm::rvsdg2jlm(rvsdg);

	llvm::LLVMContext ctx;
	auto llvm_module = jlm::jlm2llvm::convert(*jlm_module, ctx);

	if (fp == "") {
		llvm::raw_os_ostream os(std::cout);
		llvm_module->print(os, nullptr);
	} else {
		std::error_code ec;
		llvm::raw_fd_ostream os(fp.to_str(), ec);
		llvm_module->print(os, nullptr);
	}
}

static void
print(
	const jlm::rvsdg & rvsdg,
	const jlm::filepath & fp,
	const jlm::outputformat & format)
{
	static std::unordered_map<
		jlm::outputformat,
		std::function<void(const jlm::rvsdg&, const jlm::filepath&)>
	> formatters({
		{jlm::outputformat::xml,  print_as_xml}
	, {jlm::outputformat::llvm, print_as_llvm}
	});

	JLM_DEBUG_ASSERT(formatters.find(format) != formatters.end());
	formatters[format](rvsdg, fp);
}

int
main(int argc, char ** argv)
{
	jlm::cmdline_options flags;
	parse_cmdline(argc, argv, flags);

	llvm::LLVMContext ctx;
	auto llvm_module = parse_llvm_file(argv[0], flags.ifile, ctx);
	auto jlm_module = construct_jlm_module(*llvm_module);

	auto rvsdg = jlm::construct_rvsdg(*jlm_module, flags.sd);

	optimize(*rvsdg, flags.optimizations, flags.sd);

	print(*rvsdg, flags.ofile, flags.format);

	return 0;
}
