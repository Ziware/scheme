#include "scheme.h"

#include "assertions.h"
#include "basics.h"
#include "advanced.h"
#include "constants.h"
#include "error.h"
#include "heap.h"
#include "helpers.h"
#include "object.h"
#include "parser.h"
#include "scope.h"
#include "tokenizer.h"

#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace {

void ToTokens(std::vector<Token>& tokens, Object* obj) {
    if (Is<Number>(obj)) {
        tokens.emplace_back(SymbolToken{std::to_string(As<Number>(obj)->GetValue())});
        return;
    }
    if (Is<Bool>(obj)) {
        tokens.emplace_back(SymbolToken{As<Bool>(obj)->GetName()});
        return;
    }
    if (Is<Symbol>(obj)) {
        tokens.emplace_back(SymbolToken{As<Symbol>(obj)->GetName()});
        return;
    }
    ASSERT(!Is<Dot>(obj), "We must not meet dot object there");
    bool is_proper = CheckProperList(obj);
    auto list = GetList(obj);
    tokens.emplace_back(BracketToken::OPEN);
    for (size_t id = 0; id < list.size(); ++id) {
        if (id + 1 == list.size() && !is_proper) {
            tokens.emplace_back(DotToken{});
        }
        ToTokens(tokens, list[id]);
    }
    tokens.emplace_back(BracketToken::CLOSE);
}

std::string GetString(const std::vector<Token>& tokens) {
    std::string ans;
    for (size_t id = 0; id < tokens.size(); ++id) {
        if (const BracketToken* t = std::get_if<BracketToken>(&tokens[id])) {
            if (*t == BracketToken::OPEN) {
                if (id != 0) {
                    if (!std::get_if<BracketToken>(&tokens[id - 1])) {
                        ans.push_back(' ');
                    } else if (BracketToken::CLOSE == *std::get_if<BracketToken>(&tokens[id - 1])) {
                        ans.push_back(' ');
                    }
                }
                ans.push_back(kOpenBracket);
            } else {
                ans.push_back(kCloseBracket);
            }
            continue;
        }
        if (std::get_if<DotToken>(&tokens[id])) {
            if (id != 0) {
                if (!std::get_if<BracketToken>(&tokens[id - 1])) {
                    ans.push_back(' ');
                } else if (BracketToken::CLOSE == *std::get_if<BracketToken>(&tokens[id - 1])) {
                    ans.push_back(' ');
                }
            }
            ans.push_back(kDot);
            continue;
        }
        if (const SymbolToken* t = std::get_if<SymbolToken>(&tokens[id])) {
            if (id != 0) {
                if (!std::get_if<BracketToken>(&tokens[id - 1])) {
                    ans.push_back(' ');
                } else if (BracketToken::CLOSE == *std::get_if<BracketToken>(&tokens[id - 1])) {
                    ans.push_back(' ');
                }
            }
            ans += t->name;
            continue;
        }
        ASSERT(false, "We must not be there");
    }
    return ans;
}

}  // namespace

Object* Eval(Object* obj, Object* scope) {
    if (!obj) {
        throw RuntimeError("Can't evaluate empty list");
    }
    if (Is<Number>(obj)) {
        return obj;
    }
    if (Is<Bool>(obj)) {
        return obj;
    }
    if (Is<Dot>(obj)) {
        throw RuntimeError("Can't evaluate dot");
    }
    if (Is<Symbol>(obj)) {
        return As<Scope>(scope)->GetObject(As<Symbol>(obj)->GetName());
    }
    ASSERT(Is<Cell>(obj), "Unknown Object");
    
    auto cell = As<Cell>(obj);
    auto func = Eval(cell->GetFirst(), scope);
    if (!Is<Function>(func)) {
        throw RuntimeError("Unknown function");
    }
    return As<Function>(func)->Call(cell->GetSecond(), scope);
}

std::string Print(Object* obj) {
    std::vector<Token> tokens;
    ToTokens(tokens, obj);
    return GetString(tokens);
}

Interpreter::Interpreter() : global_scope_(Heap::GetHeap().Make<Scope>()) {
    Heap::GetHeap().SetGlobalScope(global_scope_);
    for (const auto& [name, func] : kAdvancedFunctions) {
        As<Scope>(global_scope_)->AddObject(name, func);
    }
    for (const auto& [name, func] : kBasicFunctions) {
        As<Scope>(global_scope_)->AddObject(name, func);
    }
    Heap::GetHeap().MarkAndSweep();
}

Interpreter::~Interpreter() {
    Heap::GetHeap().SetGlobalScope(nullptr);
}

std::string Interpreter::Run(const std::string& line) {
    std::stringstream stream{line};
    Tokenizer tokenizer(&stream);
    auto& heap = Heap::GetHeap();
    Object* root = Read(&tokenizer);
    if (!tokenizer.IsEnd()) {
        throw SyntaxError("Expected end of line at the end of command. Found: " +
                          std::to_string(static_cast<char>(stream.peek())));
    }
    auto res = Eval(root, global_scope_);
    std::string ans = Print(res);
    heap.MarkAndSweep();
    return ans;
}
