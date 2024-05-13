#pragma once

#include "object_fwd.h"

#include "scope_fwd.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

class Object {
    friend class Heap;
    friend class Cell;
    friend class Lambda;
    friend class Scope;

public:
    Object() = default;
    virtual ~Object() = default;

protected:
    virtual void Mark();
    virtual void UnMark();
    virtual bool IsMarked();

    bool marked_ = false;
};

class Number : public Object {
public:
    Number() = default;
    Number(int64_t val);
    ~Number() = default;

    int64_t GetValue() const;

private:
    int64_t value_;
};

class Bool : public Object {
public:
    Bool() = default;
    Bool(bool val);
    ~Bool() = default;

    bool GetValue() const;
    std::string GetName() const;

private:
    bool value_;
};

class Dot : public Object {
public:
    ~Dot() = default;
};

class Cell : public Object {
public:
    Object* GetFirst() const;
    Object* GetSecond() const;
    ~Cell() = default;

    template <class T>
    requires(std::is_base_of_v<Object, T>) void SetFirst(T* other) {
        cell_.first = other;
    }
    template <class T>
    requires(std::is_base_of_v<Object, T>) void SetSecond(T* other) {
        cell_.second = other;
    }

protected:
    virtual void Mark() override;
    std::pair<Object*, Object*> cell_;
};

class Symbol : public Object {
public:
    Symbol() = default;
    Symbol(std::string name);

    virtual const std::string& GetName() const;
    virtual ~Symbol() = default;

private:
    std::string name_;
};

class Function : public Symbol {
public:
    Function(const std::string& name);
    virtual ~Function() = default;

    virtual Object* Call(Object* obj, Object* scope) = 0;
};

class Lambda : public Function {
    using Signature = Object*(Object*, Object*);

public:
    Lambda(const std::string& name, Object* args, Object* body, Object* scope);
    ~Lambda() = default;

    Object* GetArgs() const;
    Object* GetBody() const;
    Object* GetScope() const;
    Object* Call(Object* obj, Object* scope) override;

protected:
    virtual void Mark() override;
    Object* args_;
    Object* body_;
    Object* scope_;
};

class Reserved : public Function {
    using Signature = Object*(Object*, Object*);

public:
    Reserved(const std::string& name, std::function<Signature> func);
    Object* Call(Object* obj, Object* scope) override;
    ~Reserved() = default;

private:
    std::function<Signature> func_;
};

//---------------------------------------------------------------------

template <class T>
requires(std::is_base_of_v<Object, T>) T* As(Object* obj) {
    return dynamic_cast<T*>(obj);
}

template <class T>
requires(std::is_base_of_v<Object, T>) bool Is(Object* obj) {
    if (dynamic_cast<T*>(obj)) {
        return true;
    }
    return false;
}
