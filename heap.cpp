#include "heap.h"

#include "object.h"
#include "scope.h"

#include <cstddef>
#include <string>
#include <vector>

std::unique_ptr<Heap> Heap::instance;

Heap& Heap::GetHeap() {
    if (!instance) {
        instance = std::unique_ptr<Heap>(new Heap());
    }
    return *instance;
}

void Heap::MarkAndSweep() {
    root_->Mark();
    std::vector<Object*> new_objects;
    for (const auto& obj : objects_) {
        if (obj->IsMarked()) {
            obj->UnMark();
            new_objects.push_back(obj);
        } else {
            delete obj;
        }
    }
    objects_ = new_objects;
}

void Heap::SetGlobalScope(Object* scope) {
    root_ = scope;
}

Heap::Heap() {
    root_ = nullptr;
}

Heap::~Heap() {
    for (const auto& obj : objects_) {
        delete obj;
    }
}
