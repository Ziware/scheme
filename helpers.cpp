#include "helpers.h"

#include "error.h"

#include <utility>

bool CheckProperList(Object* obj) {
    while (obj != nullptr) {
        if (!Is<Cell>(obj)) {
            return false;
        }
        obj = As<Cell>(obj)->GetSecond();
    }
    return true;
}

std::vector<Object*> GetProperList(Object* obj, const std::string context) {
    std::vector<Object*> list;
    while (obj != nullptr) {
        if (!Is<Cell>(obj)) {
            if (context.empty()) {
                throw RuntimeError("List must be proper");
            } else {
                throw RuntimeError("List must be proper in " + context);
            }
        }
        list.emplace_back(As<Cell>(obj)->GetFirst());
        obj = As<Cell>(obj)->GetSecond();
    }
    return list;
}

std::vector<Object*> GetList(Object* obj) {
    std::vector<Object*> list;
    while (obj != nullptr) {
        if (!Is<Cell>(obj)) {
            list.emplace_back(obj);
            break;
        }
        list.emplace_back(As<Cell>(obj)->GetFirst());
        obj = As<Cell>(obj)->GetSecond();
    }
    return list;
}

bool CheckPair(Object* obj) {
    auto list = GetList(obj);
    return list.size() == 2;
}

std::pair<Object*, Object*> GetPair(Object* obj) {
    auto list = GetList(obj);
    if (list.size() != 2) {
        throw RuntimeError("Is not a pair");
    }
    return std::make_pair(list[0], list[1]);
}

bool CheckNull(Object* obj) {
    return obj == nullptr;
}
