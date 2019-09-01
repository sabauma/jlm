/*
 * Copyright 2018 Nico Reißmann <nico.reissmann@gmail.com>
 * See COPYING for terms of redistribution.
 */

#include "test-registry.hpp"
#include "test-types.hpp"

#include <jive/rvsdg/control.h>
#include <jive/rvsdg/gamma.h>
#include <jive/view.h>

#include <jlm/ir/cfg-structure.hpp>
#include <jlm/ir/module.hpp>
#include <jlm/ir/operators.hpp>
#include <jlm/ir/print.hpp>
#include <jlm/ir/rvsdg.hpp>
#include <jlm/rvsdg2jlm/rvsdg2jlm.hpp>

static void
test_with_match()
{
	using namespace jlm;

	jlm::valuetype vt;
	jive::bittype bt1(1);
	jive::fcttype ft({&bt1, &vt, &vt}, {&vt});

	jlm::rvsdg rvsdg(filepath(""), "", "");
	auto nf = rvsdg.graph()->node_normal_form(typeid(jive::operation));
	nf->set_mutable(false);

	/* setup graph */

	jlm::lambda_builder lb;
	auto arguments = lb.begin_lambda(rvsdg.graph()->root(), {ft, "f", linkage::external_linkage});

	auto match = jive::match(1, {{0, 0}}, 1, 2, arguments[0]);
	auto gamma = jive::gamma_node::create(match, 2);
	auto ev1 = gamma->add_entryvar(arguments[1]);
	auto ev2 = gamma->add_entryvar(arguments[2]);
	auto ex = gamma->add_exitvar({ev1->argument(0), ev2->argument(1)});

	auto lambda = lb.end_lambda({ex});
	rvsdg.graph()->add_export(lambda->output(0), {lambda->output(0)->type(), ""});


	jive::view(*rvsdg.graph(), stdout);
	auto module = jlm::rvsdg2jlm::rvsdg2jlm(rvsdg);
	jlm::print(*module, stdout);

	/* verify output */

	auto & ipg = module->ipgraph();
	assert(ipg.nnodes() == 1);

	auto cfg = dynamic_cast<const jlm::function_node&>(*ipg.begin()).cfg();
	assert(cfg->nnodes() == 1);
	auto node = cfg->entry()->outedge(0)->sink();
	auto bb = dynamic_cast<const basic_block*>(node);
	assert(jive::is<jlm::select_op>(bb->tacs().last()->operation()));
}

static void
test_without_match()
{
	using namespace jlm;

	jlm::valuetype vt;
	jive::ctltype ctl2(2);
	jive::bittype bt1(1);
	jive::fcttype ft({&ctl2, &vt, &vt}, {&vt});

	jlm::rvsdg rvsdg(filepath(""), "", "");
	auto nf = rvsdg.graph()->node_normal_form(typeid(jive::operation));
	nf->set_mutable(false);

	/* setup graph */

	jlm::lambda_builder lb;
	auto arguments = lb.begin_lambda(rvsdg.graph()->root(), {ft, "f", linkage::external_linkage});

	auto gamma = jive::gamma_node::create(arguments[0], 2);
	auto ev1 = gamma->add_entryvar(arguments[1]);
	auto ev2 = gamma->add_entryvar(arguments[2]);
	auto ex = gamma->add_exitvar({ev1->argument(0), ev2->argument(1)});

	auto lambda = lb.end_lambda({ex});
	rvsdg.graph()->add_export(lambda->output(0), {lambda->output(0)->type(), ""});


	jive::view(*rvsdg.graph(), stdout);
	auto module = jlm::rvsdg2jlm::rvsdg2jlm(rvsdg);
	jlm::print(*module, stdout);

	/* verify output */

	auto & ipg = module->ipgraph();
	assert(ipg.nnodes() == 1);

	auto cfg = dynamic_cast<const jlm::function_node&>(*ipg.begin()).cfg();
	assert(cfg->nnodes() == 1);
	auto node = cfg->entry()->outedge(0)->sink();
	auto bb = dynamic_cast<const basic_block*>(node);
	assert(jive::is<jlm::ctl2bits_op>(bb->tacs().first()->operation()));
	assert(jive::is<jlm::select_op>(bb->tacs().last()->operation()));
}

static void
test_gamma3()
{
	using namespace jlm;

	jlm::valuetype vt;
	jive::fcttype ft({&jive::bit32, &vt, &vt}, {&vt});

	jlm::rvsdg rvsdg(filepath(""), "", "");
	auto nf = rvsdg.graph()->node_normal_form(typeid(jive::operation));
	nf->set_mutable(false);

	/* setup graph */

	jlm::lambda_builder lb;
	auto arguments = lb.begin_lambda(rvsdg.graph()->root(), {ft, "f", linkage::external_linkage});

	auto match = jive::match(32, {{0, 0}, {1, 1}}, 2, 3, arguments[0]);

	auto gamma = jive::gamma_node::create(match, 3);
	auto ev1 = gamma->add_entryvar(arguments[1]);
	auto ev2 = gamma->add_entryvar(arguments[2]);
	auto ex = gamma->add_exitvar({ev1->argument(0), ev1->argument(1), ev2->argument(2)});

	auto lambda = lb.end_lambda({ex});
	rvsdg.graph()->add_export(lambda->output(0), {lambda->output(0)->type(), ""});

	jive::view(*rvsdg.graph(), stdout);
	auto module = jlm::rvsdg2jlm::rvsdg2jlm(rvsdg);
	jlm::print(*module, stdout);

	/* verify output */

	auto & ipg = module->ipgraph();
	assert(ipg.nnodes() == 1);

	auto cfg = dynamic_cast<const jlm::function_node&>(*ipg.begin()).cfg();
	assert(is_closed(*cfg));
}

static int
test()
{
	test_with_match();
	test_without_match();

	test_gamma3();

	return 0;
}

JLM_UNIT_TEST_REGISTER("libjlm/r2j/test-empty-gamma", test)
