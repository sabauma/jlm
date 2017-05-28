/*
 * Copyright 2017 Nico Reißmann <nico.reissmann@gmail.com>
 * See COPYING for terms of redistribution.
 */

#include <test-operation.hpp>
#include <test-registry.hpp>
#include <test-types.hpp>

#include <jlm/IR/aggregation/aggregation.hpp>
#include <jlm/IR/aggregation/node.hpp>
#include <jlm/IR/basic_block.hpp>
#include <jlm/IR/cfg.hpp>

static void
test_linear_graph()
{
	jlm::cfg cfg;
	jlm::valuetype vtype;
	jlm::test_op op({&vtype}, {&vtype});

	auto arg = create_variable(vtype, "arg");
	auto v1 = create_variable(vtype);
	auto v2 = create_variable(vtype);
	auto bb1 = create_basic_block_node(&cfg);
	auto bb2 = create_basic_block_node(&cfg);

	cfg.exit_node()->divert_inedges(bb1);
	bb1->add_outedge(bb2, 0);
	bb2->add_outedge(cfg.exit_node(), 0);

	cfg.entry().append_argument(arg);
	static_cast<jlm::basic_block*>(&bb1->attribute())->append(create_tac(op, {arg}, {v1}));
	static_cast<jlm::basic_block*>(&bb2->attribute())->append(create_tac(op, {v1}, {v2}));
	cfg.exit().append_result(v2);

	auto root = jlm::agg::aggregate(cfg);
	jlm::agg::view(*root, stdout);

	auto dm = jlm::agg::annotate(*root);

	/* linear */
	assert(dm.find(root.get()) != dm.end());
	auto ds = dm[root.get()];
	assert(ds.empty());

	/* entry */
	assert(dm.find(root->child(0)) != dm.end());
	ds = dm[root->child(0)];
	assert(ds.empty());

	/* linear */
	assert(dm.find(root->child(1)) != dm.end());
	ds = dm[root->child(1)];
	assert(ds.size() == 1 && ds.find(arg) != ds.end());

	/* bb1 */
	assert(dm.find(root->child(1)->child(0)) != dm.end());
	ds = dm[root->child(1)->child(0)];
	assert(ds.size() == 1 && ds.find(arg) != ds.end());

	/* linear */
	assert(dm.find(root->child(1)->child(1)) != dm.end());
	ds = dm[root->child(1)->child(1)];
	assert(ds.size() == 1 && ds.find(v1) != ds.end());

	/* bb2 */
	assert(dm.find(root->child(1)->child(1)->child(0)) != dm.end());
	ds = dm[root->child(1)->child(1)->child(0)];
	assert(ds.size() == 1 && ds.find(v1) != ds.end());

	/* exit */
	assert(dm.find(root->child(1)->child(1)->child(1)) != dm.end());
	ds = dm[root->child(1)->child(1)->child(1)];
	assert(ds.size() == 1 && ds.find(v2) != ds.end());
}

static void
test_branch_graph()
{
	jlm::cfg cfg;
	jlm::valuetype vtype;
	jlm::test_op unop({&vtype}, {&vtype});
	jlm::test_op binop({&vtype, &vtype}, {&vtype});

	auto arg = create_variable(vtype, "arg");
	auto v1 = create_variable(vtype, "v1");
	auto v2 = create_variable(vtype, "v2");
	auto v3 = create_variable(vtype, "v3");
	auto v4 = create_variable(vtype, "v4");
	auto split = create_basic_block_node(&cfg);
	auto bb1 = create_basic_block_node(&cfg);
	auto bb2 = create_basic_block_node(&cfg);
	auto join = create_basic_block_node(&cfg);

	cfg.exit_node()->divert_inedges(split);
	split->add_outedge(bb1, 0);
	split->add_outedge(bb2, 1);
	bb1->add_outedge(join, 0);
	bb2->add_outedge(join, 0);
	join->add_outedge(cfg.exit_node(), 0);

	cfg.entry().append_argument(arg);
	static_cast<jlm::basic_block*>(&split->attribute())->append(create_tac(unop, {arg}, {v1}));
	static_cast<jlm::basic_block*>(&bb1->attribute())->append(create_tac(unop, {v1}, {v2}));
	static_cast<jlm::basic_block*>(&bb2->attribute())->append(create_tac(unop, {v1}, {v3}));
	static_cast<jlm::basic_block*>(&join->attribute())->append(create_tac(binop, {v2, v3}, {v4}));
	cfg.exit().append_result(v4);

	auto root = jlm::agg::aggregate(cfg);
	jlm::agg::view(*root, stdout);

	auto dm = jlm::agg::annotate(*root);

	/* linear */
	assert(dm.find(root.get()) != dm.end());
	auto ds = dm[root.get()];
	assert(ds.empty());

	/* entry */
	assert(dm.find(root->child(0)) != dm.end());
	ds = dm[root->child(0)];
	assert(ds.empty());

	/* linear */
	assert(dm.find(root->child(1)) != dm.end());
	ds = dm[root->child(1)];
	assert(ds.size() == 1 && ds.find(arg) != ds.end());

	/* branch */
	assert(dm.find(root->child(1)->child(0)) != dm.end());
	ds = dm[root->child(1)->child(0)];
	assert(ds.size() == 1 && ds.find(arg) != ds.end());

	/* bb2 */
	assert(dm.find(root->child(1)->child(0)->child(0)) != dm.end());
	ds = dm[root->child(1)->child(0)->child(0)];
	assert(ds.size() == 2 && ds.find(v1) != ds.end() && ds.find(v2) != ds.end());

	/* bb1 */
	assert(dm.find(root->child(1)->child(0)->child(1)) != dm.end());
	ds = dm[root->child(1)->child(0)->child(1)];
	assert(ds.size() == 2 && ds.find(v1) != ds.end() && ds.find(v3) != ds.end());

	/* exit */
	assert(dm.find(root->child(1)->child(1)) != dm.end());
	ds = dm[root->child(1)->child(1)];
	assert(ds.size() == 1 && ds.find(v4) != ds.end());
}

static void
test_loop_graph()
{
	jlm::cfg cfg;
	jlm::valuetype vtype;
	jlm::test_op binop({&vtype, &vtype}, {&vtype});

	auto arg = create_variable(vtype, "arg");
	auto r = create_variable(vtype, "r");
	auto bb = create_basic_block_node(&cfg);

	cfg.exit_node()->divert_inedges(bb);
	bb->add_outedge(cfg.exit_node(), 0);
	bb->add_outedge(bb, 1);

	cfg.entry().append_argument(arg);
	static_cast<jlm::basic_block*>(&bb->attribute())->append(create_tac(binop, {arg, r}, {r}));
	cfg.exit().append_result(r);

	auto root = jlm::agg::aggregate(cfg);
	jlm::agg::view(*root, stdout);

	auto dm = jlm::agg::annotate(*root);

	/* linear */
	assert(dm.find(root.get()) != dm.end());
	auto ds = dm[root.get()];
	assert(ds.size() == 1 && ds.find(r) != ds.end());

	/* entry */
	assert(dm.find(root->child(0)) != dm.end());
	ds = dm[root->child(0)];
	assert(ds.size() == 1 && ds.find(r) != ds.end());

	/* linear */
	assert(dm.find(root->child(1)) != dm.end());
	ds = dm[root->child(1)];
	assert(ds.size() == 2 && ds.find(arg) != ds.end() && ds.find(r) != ds.end());

	/* loop */
	assert(dm.find(root->child(1)->child(0)) != dm.end());
	ds = dm[root->child(1)->child(0)];
	assert(ds.size() == 2 && ds.find(arg) != ds.end() && ds.find(r) != ds.end());

	/* bb2 */
	assert(dm.find(root->child(1)->child(0)->child(0)) != dm.end());
	ds = dm[root->child(1)->child(0)->child(0)];
	assert(ds.size() == 2 && ds.find(arg) != ds.end() && ds.find(r) != ds.end());

	/* exit */
	assert(dm.find(root->child(1)->child(1)) != dm.end());
	ds = dm[root->child(1)->child(1)];
	assert(ds.size() == 1 && ds.find(r) != ds.end());
}

static int
test(const jive::graph * graph)
{
	/* FIXME: avoid aggregation function and build aggregated tree directly */
	test_linear_graph();
	test_branch_graph();
	test_loop_graph();

	return 0;
}

JLM_UNIT_TEST_REGISTER("libjlm/test-annotation", nullptr, test);