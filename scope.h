#pragma once

#include "scope_fwd.h"

#include "object.h"

#include <map>
#include <memory>
#include <string>

class Scope : public Object {
    friend class Heap;

public:
    Scope();
    Scope(Object* other);
    Scope(Scope& other) = delete;
    Scope(Scope&& other) = delete;
    Scope& operator=(const Scope& other) = delete;
    Scope& operator=(Scope&& other) = delete;

    virtual ~Scope() = default;

    Object* GetObject(const std::string& name);
    void SetObject(const std::string& name, Object* object);
    void AddObject(const std::string& name, Object* object);

protected:
    Scope* prev_scope_;
    std::map<std::string, Object*> objects_;

    bool IsGlobal() const;
    virtual void Mark() override;
};
