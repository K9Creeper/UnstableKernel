#include "ordered_array.hpp"

template <typename type_t>
OrderedArray<type_t>::~OrderedArray() {}

template <typename type_t>
void OrderedArray<type_t>::RePlace(void *address, uint32_t max_size)
{
	array = reinterpret_cast<type_t *>(address);
	memset(reinterpret_cast<uint8_t *>(address), 0, max_size * sizeof(type_t));

	size = 0;
	this->max_size = max_size;
}

template <typename type_t>
void OrderedArray<type_t>::Insert(type_t item)
{
	uint32_t i;
	static type_t tmp, tmp2;

	// Iterate till find value greater or equal to item
	i = 0;

	while (i < this->size && this->lessthan_(this->array[i], item)) // array[i] < item
	{
		i++;
	}

	// If reached end of array, just append the item
	if (i == this->size)
	{
		this->array[this->size] = item;

		this->size++;
	}
	else // insert
	{
		tmp = this->array[i];

		this->array[i] = item;

		while (i < this->size)
		{
			i++;

			tmp2 = this->array[i];

			this->array[i] = tmp;

			tmp = tmp2;
		}

		this->size++;
	}
}

template <typename type_t>
type_t OrderedArray<type_t>::Get(uint32_t i)
{
	return this->array[i];
}

template <typename type_t>
void OrderedArray<type_t>::Remove(uint32_t i)
{
	while (i < array->size)
	{
		array->array[i] = array->array[i + 1];

		i += 1;
	}

	array->size -= 1;
}

template <typename type_t>
uint32_t OrderedArray<type_t>::GetSize() const
{
	return size;
}
template <typename type_t>
uint32_t OrderedArray<type_t>::GetMaxSize() const
{
	return max_size;
}