#pragma once

#include "object.h"
#include "scope.h"

#include <string>
#include <memory>

Object* Eval(Object* obj, Object* scope);

std::string Print(Object* obj);

class Interpreter {
public:
    Interpreter();
    ~Interpreter();
    std::string Run(const std::string& line);

private:
    Object* global_scope_;
};
