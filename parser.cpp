#include "parser.h"

#include "assertions.h"
#include "constants.h"
#include "error.h"
#include "object.h"
#include "scheme.h"
#include "heap.h"

#include <memory>
#include <variant>
#include <vector>

namespace {

// (... objects, [.], object) dot must be only one and locate before last symbol
Object* GetObjectByList(std::vector<Object*>& list) {
    if (list.empty()) {
        return nullptr;
    }
    if (Is<Dot>(list[0])) {
        throw SyntaxError("Dot can't be first symbol in list\n" + kDotPositionHelp);
    } else if (Is<Dot>(list.back())) {
        throw SyntaxError("Dot can't be last symbol in list\n" + kDotPositionHelp);
    }
    Cell* to_retern = As<Cell>(Heap::GetHeap().Make<Cell>());
    to_retern->SetFirst(list[0]);

    auto cur(to_retern);
    for (size_t id = 1; id < list.size(); ++id) {
        if (Is<Dot>(list[id])) {
            if (id + 2 != list.size()) {
                throw SyntaxError(kDotPositionHelp);
            }
            cur->SetSecond(list[id + 1]);
            break;
        }
        cur->SetSecond(Heap::GetHeap().Make<Cell>());
        cur = As<Cell>(cur->GetSecond());
        cur->SetFirst(list[id]);
    }
    return to_retern;
}

}  // namespace

Object* Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Empty expression is invald");
    }
    auto token = tokenizer->GetToken();
    if (std::get_if<BracketToken>(&token)) {
        return ReadList(tokenizer);
    }
    tokenizer->Next();

    if (ConstantToken* ptr = std::get_if<ConstantToken>(&token)) {
        return Heap::GetHeap().Make<Number>(ptr->value);
    }
    if (BooleanToken* ptr = std::get_if<BooleanToken>(&token)) {
        return Heap::GetHeap().Make<Bool>(*ptr == BooleanToken::TRUE);
    }
    if (std::get_if<DotToken>(&token)) {
        return Heap::GetHeap().Make<Dot>();
    }
    if (SymbolToken* ptr = std::get_if<SymbolToken>(&token)) {
        auto name = ptr->name;
        return Heap::GetHeap().Make<Symbol>(name);
    }
    if (std::get_if<QuoteToken>(&token)) {
        Object* obj = Heap::GetHeap().Make<Cell>();
        As<Cell>(obj)->SetFirst(Heap::GetHeap().Make<Symbol>(kQuote));
        As<Cell>(obj)->SetSecond(Heap::GetHeap().Make<Cell>());
        As<Cell>(As<Cell>(obj)->GetSecond())->SetFirst(Read(tokenizer));
        if (Is<Dot>(As<Cell>(As<Cell>(obj)->GetSecond())->GetFirst())) {
            throw SyntaxError("Quote operation can't be done on dot");
        }
        return obj;
    }
    ASSERT(false, "Unknown Token");
    return nullptr;  //  should not be there
}

Object* ReadList(Tokenizer* tokenizer) {
    std::vector<Object*> list;

    {
        auto token = tokenizer->GetToken();
        if (BracketToken* t = std::get_if<BracketToken>(&token)) {
            if (*t == BracketToken::CLOSE) {
                throw SyntaxError("Bracket balance become negative\n" + kBracketHelp);
            }
        }
        tokenizer->Next();
    }

    while (true) {  // while not matched ')' token
        if (tokenizer->IsEnd()) {
            throw SyntaxError("Final bracket balance not equal 0\n" + kBracketHelp);
        }
        auto token = tokenizer->GetToken();
        if (BracketToken* t = std::get_if<BracketToken>(&token)) {
            if (*t == BracketToken::CLOSE) {
                tokenizer->Next();
                break;
            }
            list.emplace_back(ReadList(tokenizer));
            continue;
        }
        try {
            list.emplace_back(Read(tokenizer));
        } catch (SyntaxError& ex) {
            throw SyntaxError("Final bracket balance not equal 0\n" + kBracketHelp);
        }
    }

    return GetObjectByList(list);
}
