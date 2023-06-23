#pragma once

#include <vector>

#include "expression.hpp"
#include "token.hpp"
#include "visitor.hpp"

using namespace std;

namespace statement {
    template <class> class Visitor;

    class Statement {
        public:
            template <class T>
            T accept(Visitor<T> v) { }
    };

    class Block : Statement {
        public:
            const vector<Statement> stmts;

            Block(vector<Statement> stmts) : stmts(stmts) { }

            template <class T>
            T accept(Visitor<T> v) { return v.block(this); }
    };

    class Function : Statement {
        public:
            const token::Token name;
            const vector<token::Token> params;
            const vector<Statement> body;

            Function(token::Token name, vector<token::Token> params, vector<Statement> body) : name(name), params(params), body(body) { }

            template <class T>
            T accept(Visitor<T> v) { return v.function(this); }
    };

    class Class : Statement {
        public:
            const token::Token name;
            const expression::Variable superclass;
            const vector<Function> methods;

            Class(token::Token name, expression::Variable superclass, vector<Function> methods) : name(name), superclass(superclass), methods(methods) { }

            template <class T>
            T accept(Visitor<T> v) { return v._class(this); }
    };

    class Expression : Statement {
        public:
            const expression::Expression exp;

            Expression(expression::Expression exp) : exp(exp) { }

            template <class T>
            T accept(Visitor<T> v) { return v.expression(this); }
    };

    class If : Statement {
        public:
            const expression::Expression condition;
            const Statement then_branch;
            const Statement else_branch;

            If(expression::Expression condition, Statement then_branch, Statement else_branch) : condition(condition), then_branch(then_branch), else_branch(else_branch) { }

            template <class T>
            T accept(Visitor<T> v) { return v._if(this); }
    };

    class Return : Statement {
        public:
            const token::Token keyword;
            const expression::Expression value;

            Return(token::Token keyword, expression::Expression value) : keyword(keyword), value(value) { }

            template <class T>
            T accept(Visitor<T> v) { return v._return(this); }
    };

    class Let : Statement {
        public:
            const token::Token name;
            const expression::Expression init;

            Let(token::Token name, expression::Expression init) : name(name), init(init) { }

            template <class T>
            T accept(Visitor<T> v) { return v.let(this); }
    };

    class For : Statement {
        public:
            const expression::Expression condition;
            const Statement body;

            For(expression::Expression condition, Statement body) : condition(condition), body(body) { }

            template <class T>
            T accept(Visitor<T> v) { return v._for(this); }
    };
};
