#include "object.h"

#include "constants.h"
#include "advanced.h"
#include "heap.h"
#include "helpers.h"
#include "scope.h"

#include <memory>
#include <type_traits>
#include <vector>

void Object::Mark() {
    marked_ = true;
}

void Object::UnMark() {
    marked_ = false;
}

bool Object::IsMarked() {
    return marked_;
}

Number::Number(int64_t val) : value_(val){};

int64_t Number::GetValue() const {
    return value_;
}

Bool::Bool(bool val) : value_(val){};

bool Bool::GetValue() const {
    return value_;
}

std::string Bool::GetName() const {
    if (value_) {
        return kTrue;
    } else {
        return kFalse;
    }
}

Object* Cell::GetFirst() const {
    return cell_.first;
}

Object* Cell::GetSecond() const {
    return cell_.second;
}

void Cell::Mark() {
    if (marked_) {
        return;
    }
    marked_ = true;
    if (cell_.first) {
        cell_.first->Mark();
    }
    if (cell_.second) {
        cell_.second->Mark();
    }
}

Symbol::Symbol(std::string name) : name_(name){};

const std::string& Symbol::GetName() const {
    return name_;
}

Function::Function(const std::string& name) : Symbol(name){};

Lambda::Lambda(const std::string& name, Object* args, Object* body, Object* scope)
    : Function(name), args_(args), body_(body), scope_(scope){};

Object* Lambda::GetArgs() const {
    return args_;
}

Object* Lambda::GetBody() const {
    return body_;
}

Object* Lambda::GetScope() const {
    return scope_;
}

Object* Lambda::Call(Object* obj, Object* scope) {
    auto res =
        advanced::FEvalLambda(GetProperList(GetArgs(), GetName()), GetProperList(obj, GetName()),
                              GetProperList(GetBody()), GetScope(), scope);
    return res;
}

void Lambda::Mark() {
    if (marked_) {
        return;
    }
    marked_ = true;
    if (args_) {
        args_->Mark();
    }
    if (body_) {
        body_->Mark();
    }
    if (scope_) {
        scope_->Mark();
    }
}

Reserved::Reserved(const std::string& name, std::function<Signature> func)
    : Function(name), func_(func) {
}

Object* Reserved::Call(Object* obj, Object* scope) {
    auto res = func_(obj, scope);
    return res;
}
