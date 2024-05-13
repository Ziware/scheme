#pragma once

#include "object_fwd.h"

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

class Heap {
public:
    static Heap& GetHeap();
    Heap(const Heap& other) = delete;
    Heap(Heap&& other) = delete;
    Heap& operator=(const Heap& other) = delete;
    Heap& operator=(Heap&& other) = delete;

    template <class T, class... Args>
    requires std::is_base_of_v<Object, T> Object* Make(Args&&... args) {
        objects_.emplace_back(new T(std::forward<Args>(args)...));
        return objects_.back();
    }
    template <class T>
    requires std::is_base_of_v<Object, T> Object* Clone(T* obj) {
        objects_.emplace_back(new T(*obj));
        return objects_.back();
    }

    void SetGlobalScope(Object* scope);

    void MarkAndSweep();

    ~Heap();

private:
    Heap();
    std::vector<Object*> objects_;
    Object* root_;
    static std::unique_ptr<Heap> instance;
};
