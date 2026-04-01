// You need to submit this file
#pragma once

#include <any>

// The evaluator relies on the visitor interface and node classes
// defined by the judge (visitor.h). We only implement the calculator.
struct visitor;
struct node;
struct num_node;
struct add_node;
struct sub_node;
struct mul_node;
struct div_node;

struct calculator : visitor {
    std::any visit_num(num_node *n) override {
        return n->number; // preserve original type (long long or double)
    }

    std::any visit_add(add_node *n) override { return binary_op(n->lnode, n->rnode, [](auto a, auto b){ return a + b; }); }
    std::any visit_sub(sub_node *n) override { return binary_op(n->lnode, n->rnode, [](auto a, auto b){ return a - b; }); }
    std::any visit_mul(mul_node *n) override { return binary_op(n->lnode, n->rnode, [](auto a, auto b){ return a * b; }); }
    std::any visit_div(div_node *n) override { return binary_op(n->lnode, n->rnode, [](auto a, auto b){ return a / b; }); }

    ~calculator() override = default;

private:
    template <class F>
    std::any binary_op(node *l, node *r, F fn) {
        auto lv = visit(l);
        auto rv = visit(r);

        if (auto lp = std::any_cast<long long>(&lv)) {
            if (auto rp = std::any_cast<long long>(&rv)) {
                return std::any(fn(*lp, *rp));
            }
        }
        // If not both long long, treat as double. We assume trees are homogeneous,
        // but this also tolerates mixed types by promoting to double.
        double ld = std::any_cast<double>(lv);
        double rd = std::any_cast<double>(rv);
        return std::any(fn(ld, rd));
    }
};

