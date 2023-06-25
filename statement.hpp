#pragma once

#include <vector>

#include "expression.hpp"
#include "token.hpp"

using namespace std;

namespace statement {
    template <class> class Visitor;

    class Statement {
        public:
            template <class T>
            T accept(Visitor<T> v) { }
    };

    class Block : public Statement {
        public:
            const vector<Statement> stmts;

            Block(vector<Statement> stmts) : stmts(stmts) { }

            template <class T>
            T accept(Visitor<T> v) { return v.visit_block(this); }
    };

    class Function : public Statement {
        public:
            const token::Token name;
            const vector<token::Token> params;
            const vector<Statement> body;

            Function(token::Token name, vector<token::Token> params, vector<Statement> body) : name(name), params(params), body(body) { }

            template <class T>
            T accept(Visitor<T> v) { return v.visit_function(this); }
    };

    class Class : public Statement {
        public:
            const token::Token name;
            const expression::Variable* superclass;
            const vector<Function> methods;

            Class(token::Token name, expression::Variable* superclass, vector<Function> methods) : name(name), superclass(superclass), methods(methods) { }
            ~Class() { delete this->superclass; }

            template <class T>
            T accept(Visitor<T> v) { return v.visit_class(this); }
    };

    class Expression : public Statement {
        public:
            const expression::Expression exp;

            Expression(expression::Expression exp) : exp(exp) { }

            template <class T>
            T accept(Visitor<T> v) { return v.visit_expression(this); }
    };

    class If : public Statement {
        public:
            const expression::Expression condition;
            const Statement* then_branch;
            const Statement* else_branch;

            If(expression::Expression condition, Statement* then_branch, Statement* else_branch) : condition(condition), then_branch(then_branch), else_branch(else_branch) { }
            ~If() {
                delete this->then_branch;
                delete this->else_branch;
            }

            template <class T>
            T accept(Visitor<T> v) { return v.visit_if(this); }
    };

    class Return : public Statement {
        public:
            const token::Token keyword;
            const expression::Expression* value;

            Return(token::Token keyword, expression::Expression* value) : keyword(keyword), value(value) { }
            ~Return() { delete this->value; }

            template <class T>
            T accept(Visitor<T> v) { return v.visit_return(this); }
    };

    class Let : public Statement {
        public:
            const token::Token name;
            const expression::Expression* init;

            Let(token::Token name, expression::Expression* init) : name(name), init(init) { }
            ~Let() { delete this->init; }

            template <class T>
            T accept(Visitor<T> v) { return v.visit_let(this); }
    };

    class For : public Statement {
        public:
            const expression::Expression condition;
            const Statement body;

            For(expression::Expression condition, Statement body) : condition(condition), body(body) { }

            template <class T>
            T accept(Visitor<T> v) { return v.visit_for(this); }
    };

    class Use : public Statement {
        public:
            const token::Token name;

            Use(token::Token name) : name(name) { }

            template <class T>
            T accept(Visitor<T> v) { return v.visit_use(this); }
    };
};
