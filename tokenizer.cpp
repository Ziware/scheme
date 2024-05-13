#include "tokenizer.h"

#include "assertions.h"
#include "constants.h"
#include "error.h"

#include <cctype>
#include <cstdio>
#include <stdexcept>
#include <string>

namespace {

const std::map<char, Token> kSpecial = {
    {kQuoteChar, QuoteToken()},
    {kDot, DotToken()},
    {kOpenBracket, BracketToken::OPEN},
    {kCloseBracket, BracketToken::CLOSE},
};

bool IsSpecial(char c) {
    return kSpecial.find(c) != kSpecial.end();
}

Token GetSpecial(char c) {
    return kSpecial.at(c);
}

bool IsBoolean(const std::string& s) {
    return s == kFalse || s == kTrue;
}

Token GetBoolean(const std::string& s) {
    if (s == kFalse) {
        return BooleanToken::FALSE;
    } else {
        return BooleanToken::TRUE;
    }
}

bool IsConstant(const std::string& s) {
    if (s.empty()) {
        return false;
    }
    size_t id = s.size() - 1;
    while (id > 0 && std::isdigit(s[id])) {
        --id;
    }
    if (id != 0) {
        return false;
    }
    if (std::isdigit(s[id]) || (s.size() >= 2 && (s[id] == '+' || s[id] == '-'))) {
        return true;
    }
    return false;
}

bool IsStrange(char c) {
    return c == '<' || c == '=' || c == '>' || c == '*' || c == '/' || c == '#';
}

bool IsStrangePlus(char c) {
    return IsStrange(c) || c == '?' || c == '!' || c == '-';
}

bool IsSymbol(const std::string& s) {
    // we can use there regex, but it slow
    // (^[+-]$)|(^[a-zA-Z<=>*/#][a-zA-Z<=>*/#0-9?!-]*$)
    if (s == "+" || s == "-") {
        return true;
    }
    if (!(std::isalpha(s[0]) || IsStrange(s[0]))) {
        return false;
    }
    for (size_t id = 1; id < s.size(); ++id) {
        if (!(std::isalnum(s[id]) || IsStrangePlus(s[id]))) {
            return false;
        }
    }
    return true;
}

}  // namespace

bool SymbolToken::operator==(const SymbolToken& other) const {
    return name == other.name;
}

bool QuoteToken::operator==(const QuoteToken&) const {
    return true;
}

bool DotToken::operator==(const DotToken&) const {
    return true;
}

bool ConstantToken::operator==(const ConstantToken& other) const {
    return value == other.value;
}

Tokenizer::Tokenizer(std::istream* in) : is_end_(false), in_(in) {
    Next();
};

bool Tokenizer::IsEnd() const {
    return is_end_;
}

void Tokenizer::Next() {
    std::string str;
    while (in_->peek() != EOF && std::isspace(in_->peek())) {
        in_->get();
    }
    if (in_->peek() == EOF) {
        is_end_ = true;
        return;
    }
    // check for 1 char special symblos
    if (IsSpecial(in_->peek())) {
        current_token_ = GetSpecial(in_->get());
        return;
    }
    while (in_->peek() != EOF && !std::isspace(in_->peek()) && !IsSpecial(in_->peek())) {
        if (IsConstant(str) && !std::isdigit(in_->peek())) {
            break;
        }
        str.push_back(in_->get());
    }
    ASSERT(!str.empty(), "Parsing string must not be empty");

    if (IsConstant(str)) {
        // fetch constant
        try {
            current_token_ = ConstantToken{std::stoi(str)};
            return;
        } catch (std::out_of_range& ex) {
            throw SyntaxError("Too big constant: " + str);
        } catch (std::invalid_argument& ex) {
            ASSERT(false, "Function IsConstant work incorrect");
        }
    }

    if (IsBoolean(str)) {
        current_token_ = GetBoolean(str);
        return;
    }
    if (IsSymbol(str)) {
        current_token_ = SymbolToken{str};
        return;
    }
    throw SyntaxError("Forbidden name: " + str);
}

Token Tokenizer::GetToken() {
    return current_token_;
}
