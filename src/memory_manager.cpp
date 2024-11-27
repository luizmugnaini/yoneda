///                             Yoneda library
/// Copyright (C) 2024 Luiz Gustavo Mugnaini Anselmo
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy of
/// this software and associated documentation files (the “Software”), to deal in
/// the Software without restriction, including without limitation the rights to
/// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
/// of the Software, and to permit persons to whom the Software is furnished to do
/// so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///
/// Description: Implementation of the memory allocation manager.
/// Author: Luiz G. Mugnaini A. <luizmugnaini@gmail.com>

#include <psh/memory_manager.hpp>

#include <psh/memory.hpp>

namespace psh {
    void MemoryManager::init(usize capacity_bytes) {
        this->allocation_count = 0;
        this->allocator.init(memory_virtual_alloc(capacity_bytes));
    }

    void MemoryManager::destroy() {
        memory_virtual_free({this->allocator.buf, this->allocator.capacity});
    }

    Option<Arena> MemoryManager::make_arena(usize size) {
        u8* memory = this->alloc<u8>(size);
        return (memory == nullptr) ? Option<Arena>{} : Option<Arena>{Arena{memory, size}};
    }

    Status MemoryManager::pop() {
        Status st = this->allocator.pop();
        if (psh_likely(st == STATUS_OK)) {
            --this->allocation_count;
        }
        return st;
    }

    Status MemoryManager::clear_until(u8 const* block) {
        u8 const* memory_start = this->allocator.buf;

        // Check if the block lies within the allocator's memory.
        if (psh_unlikely((block < memory_start) || (block > memory_start + this->allocator.previous_offset))) {
            cstring fail_reason =
                (block > memory_start + this->allocator.capacity)
                    ? "MemoryManager::clear_until called with a pointer outside of the stack "
                      "memory_start region."
                    : "MemoryManager::clear_until called with a pointer to an already free region "
                      "of the stack memory_start.";
            psh_log_error(fail_reason);
            return STATUS_FAILED;
        }

        // Pop the top memory block until popping `block` or reaching the end of the allocator.
        //
        // @NOTE: If we were given the incorrect address, we end up clearing the whole memory.
        for (;;) {
            u8 const* top_block = this->allocator.top();
            if (psh_unlikely(top_block == memory_start)) {
                break;
            }
            if (psh_likely(this->allocator.pop() == STATUS_OK)) {
                --this->allocation_count;
            }
            if (psh_unlikely(top_block == block)) {
                break;
            }
        }

        return STATUS_OK;
    }

    void MemoryManager::clear() {
        this->allocation_count = 0;
        this->allocator.clear();
    }
}  // namespace psh
