#pragma once

#include <stdint.h>

template <typename type_t>
class OrderedArray{
	protected:
		type_t* array;

		uint32_t size;
		uint32_t max_size;
	public:
		OrderedArray() : array{nullptr}, size{0}, max_size{0}{}
		~OrderedArray();

		void RePlace(void *addr, uint32_t max_size);
		void Insert(type_t item);

		type_t Get(uint32_t i);
		void Remove(uint32_t i);

		uint32_t GetSize()const;
		uint32_t GetMaxSize()const;

	private:
		virtual bool lessthan_(type_t, type_t) = 0;
};