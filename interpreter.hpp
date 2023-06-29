#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "expression.hpp"
#include "statement.hpp"
#include "error.hpp"

using namespace std;

namespace interpreter {
    class Instance;
    class Var;
    class Scope;

    class Interpreter {
        Scope* scope;

        public:
            Interpreter();
            ~Interpreter();
            void run(vector<statement::Statement*>);

        protected:
            Var* evaluate(expression::Expression*);
            void execute(statement::Statement*);
            void execute_block(vector<statement::Statement*>, Scope*);

        private:
            void check_number_ops(token::Token*, Var*);
            void check_number_ops(token::Token*, Var*, Var*);

            // Expressions
            Var* visit_assign(expression::Assign*);
            Var* visit_binary(expression::Binary*);
            Var* visit_call(expression::Call*);
            Var* visit_get(expression::Get*);
            Var* visit_set(expression::Set*);
            Var* visit_grouping(expression::Grouping*);
            Var* visit_literal(expression::Literal*);
            Var* visit_logical(expression::Logical*);
            Var* visit_super(expression::Super*);
            Var* visit_self(expression::Self*);
            Var* visit_unary(expression::Unary*);
            Var* visit_variable(expression::Variable*);

            // Statements
            void visit_block(statement::Block*);
            void visit_function(statement::Function*);
            void visit_class(statement::Class*);
            void visit_expression(statement::Expression*);
            void visit_if(statement::If*);
            void visit_print(statement::Print*);
            void visit_return(statement::Return*);
            void visit_let(statement::Let*);
            void visit_for(statement::For*);
            void visit_use(statement::Use*);
    };

    class Function : public Interpreter {
        public:
            statement::Function* declaration;
            Scope* parent;

            Function(statement::Function*, Scope*);

            string to_string();
            Function* bind(Instance*);
            Var* call(vector<Var*>);
    };

    class Class : public Interpreter {
        public:
            const string name;
            unordered_map<string, Function*> methods;
            Class* super;

            Class(string, Class*, unordered_map<string, Function*>);
            ~Class();

            string to_string();
            Function* get_method(string);
    };

    class Instance {
        Class* _class;
        unordered_map<string, Var*> _fields;

        public:
            Instance(Class*);
            ~Instance();

            string to_string();
            void set(token::Token*, Var*);
            Var* get(token::Token*);
    };

    enum class Type {
        String,
        Number,
        Bool,
        Class,
        Function,
        Instance,
        Nil
    };

    Type type_info_to_type(const type_info&);
    string type_to_string(Type);
    Type token_type_to_type(token::Type);

    class Var {
        public:
            Type type;
            bool is_const = false;
            bool is_optional = false;
            any value;

            Var(Type);
            Var(Type, any);
            Var(Type, any, bool, bool);

            void nil();
            bool is_nil();
            
            bool is_string();
            string to_string();
            
            bool is_number();
            float to_number();

            bool is_bool();
            bool to_bool();

            bool is_class();
            Class* to_class();

            bool is_instance();
            Instance* to_instance();

            bool is_function();
            Function* to_function();

            bool is_truthy();
            bool operator==(Var&);
            bool operator!=(Var&);
            bool operator>(Var&);
            bool operator>=(Var&);
            bool operator<(Var&);
            bool operator<=(Var&);
            float operator+(Var&);
            float operator+=(Var&);
            float operator-();
            float operator-(Var&);
            float operator-=(Var&);
            float operator*(Var&);
            float operator*=(Var&);
            float operator/(Var&);
            float operator/=(Var&);
    };

    class Scope {
        unordered_map<string, Var*> _values;

        public:
            Scope* parent;

            Scope() { }
            Scope(Scope* parent) : parent(parent) { }
            ~Scope();

            Var* get(string);
            bool has(string);
            void define(string, Var*);
            void assign(string, Var*);
    };

    class Return : exception {
        public:
            statement::Return* stmt;
            Var* value;

            Return(statement::Return* stmt, Var* value) : stmt(stmt), value(value) { }

            const string what() {
                return "<return " + this->value->to_string() + ">";
            }
    };
};

#endif