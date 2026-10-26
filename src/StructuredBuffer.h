#pragma once
#include "VG/VG.h"
#include <cassert>

template <typename T> class StructuredBuffer {
    vg::Flags<vg::BufferUsage> bufferUsage;
    vg::Buffer buffer;
    T *mapped = nullptr;
    int count = 0;
    int capacity = 0;

  public:
    StructuredBuffer(vg::Flags<vg::BufferUsage> bufferUsage, int capacity) : bufferUsage(bufferUsage) {
        Reserve(capacity + 1);
    }

    void Reserve(int newCapacity) {
        if (newCapacity <= capacity) return;

        capacity = newCapacity;

        vg::currentDevice->WaitUntilIdle();
        vg::Buffer newBuffer(capacity * sizeof(T), bufferUsage);
        vg::Allocate(newBuffer, {vg::MemoryProperty::HostCached, vg::MemoryProperty::HostVisible});
        T *newMapped = (T *)newBuffer.MapMemory();
        if (count > 0) memcpy(newMapped, mapped, count * sizeof(T));

        buffer = std::move(newBuffer);
        mapped = newMapped;
    }

    void Resize(int newSize) {
        Reserve(newSize);
        count = newSize;
    }

    void Add(const T &value) {
        if (count >= capacity) Reserve(capacity == 0 ? 4 : capacity * 2);

        mapped[count++] = value;
    }

    void Remove(int index) {
        assert(index >= 0 && index < count);

        for (int i = index; i < count - 1; i++) { mapped[i] = mapped[i + 1]; }

        count--;
    }

    void Pop() {
        assert(count > 0);
        count--;
    }

    int GetSize() const { return count; }

    int GetCapacity() const { return capacity; }

    T *Data() { return mapped; }

    const T *Data() const { return mapped; }

    vg::Buffer &GetBuffer() { return buffer; }

    const vg::Buffer &GetBuffer() const { return buffer; }
};
