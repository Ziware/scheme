#include "scope.h"

#include "error.h"
#include "object.h"
#include "scheme.h"

Scope::Scope() : prev_scope_(nullptr){};

Scope::Scope(Object* other) : prev_scope_(As<Scope>(other)){};

Object* Scope::GetObject(const std::string& name) {
    auto it = objects_.find(name);
    if (it == objects_.end()) {
        if (prev_scope_ == nullptr) {
            throw NameError("Can't find object \'" + name + "\'");
        }
        return prev_scope_->GetObject(name);
    }
    return it->second;
}

void Scope::SetObject(const std::string& name, Object* object) {
    auto it = objects_.find(name);
    if (it == objects_.end()) {
        if (prev_scope_ == nullptr) {
            throw NameError("Can't find object \'" + name + "\'");
        }
        prev_scope_->SetObject(name, object);
        return;
    }
    objects_[name] = object;
}

void Scope::AddObject(const std::string& name, Object* object) {
    objects_[name] = object;
}

bool Scope::IsGlobal() const {
    return prev_scope_ == nullptr;
}

void Scope::Mark() {
    if (marked_) {
        return;
    }
    marked_ = true;
    if (prev_scope_) {
        prev_scope_->Mark();
    }
    for (const auto& obj : objects_) {
        if (obj.second) {
            obj.second->Mark();
        }
    }
}
