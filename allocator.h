#ifndef __LMSTL_ALLOCATOR_H__
#define __LMSTL_ALLOCATOR_H__

#include <new>
#include <stddef.h>
#include <iostream>

namespace lmstl {

    template <typename T>
    class allocator {
    public:

        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

        static pointer allocate(size_type);

        static void deallocate(pointer);

        static void construct(pointer);
        static void construct(pointer, const_reference);

        static void destroy(pointer);

    };

    template <typename T>
    allocator<T>::pointer allocator<T>::allocate(size_type n) {
        pointer ret = static_cast<pointer>(::operator new(n * sizeof(T)));
        if (!ret) {
            std::cerr << "Out of memory" << std::endl;
            exit(1);
        }
        return ret;
    }

    template <typename T>
    void allocator<T>::deallocate(pointer target) {
        if (!target)
            return;
        ::operator delete(static_cast<void*>(target));
    }

    template <typename T>
    void allocator<T>::construct(pointer target) {
        new(target) value_type();
    }

    template <typename T>
    void allocator<T>::construct(pointer target, const_reference val) {
        new(target) value_type(val);
    }

    template <typename T>
    void allocator<T>::destroy(pointer target) {
        target->~T();
    }

    class allocator<void> {
    public:
        typedef void* pointer;
    };

}

#endif