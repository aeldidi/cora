#ifndef CORA_H
#define CORA_H
#include <cstddef>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum cora_type {
	CORA_NIL = 0,
	CORA_INT,
	CORA_FLOAT,
	CORA_CHAR,
	CORA_STRING,
	CORA_LIST,
	CORA_MAP,
	CORA_BOOL,
};

// A cora_state must be initialized by setting try_expand_memory.
struct cora_state {
	// Tries to return a new buffer new_len bytes long, where the first
	// memory_len (or (memory_len - (memory_len - new_len)) if
	// new_len < memory_len) bytes from memory are copied into the new
	// buffer. The parameter state is always passed as the state which
	// called the function.
	//
	// If new_len is 0, memory should be freed.
	//
	// Returns NULL if the memory cannot be expanded.
	void* (*try_expand_memory)(
			struct cora_state* state, const size_t new_len);

	size_t   memory_len;
	uint8_t* memory;

	// Each object is an index into memory.
	size_t* objects;

	// Maps symbols to values. The convention is that symbols[i] is defined
	// as values[i] in cora code.
	size_t* symbols;
	size_t* values;
};

// Returns 0 on success, and an error code on failure.
int cora_run(struct cora_state* state, uint8_t* source);

// Defines the function func in the cora environment with the specified name.
// If func_name is already defined in the cora environment, its definition is
// overwritten.
//
// Returns 0 on success and CORA_NO_MEMORY if the memory for the symbol
// couldn't be allocated.
int cora_define_function(const char* func_name,
		size_t (*func)(struct cora_state* state));

// A cora function returns a size_t (the object which is returned from
// function) and takes the current cora_state as a parameter. If the function
// isn't meant to return anything, return CORA_NIL.
struct cora_module_def {
	const char* name;
	size_t (*func)(struct cora_state* state);
};

// Defines a module containing the functions specified in the array of
// struct cora_module_def.
//
// Returns 0 on success and CORA_NO_MEMORY if the memory for the module
// couldn't be allocated.
int cora_define_module(const char* module_name, const size_t defs_len,
		const struct cora_module_def* defs);

// Allocates a cora_int initialized to x and places it into out.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the int
// couldn't be allocated.
bool cora_int(struct cora_state* state, size_t* out, const int64_t x);

// Allocates a cora_float initialized to x and places it into out.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the float
// couldn't be allocated.
bool cora_float(struct cora_state* state, size_t* out, const double x);

// Allocates a cora_char initialized to x and places it into out.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the char
// couldn't be allocated.
bool cora_char(struct cora_state* state, size_t* out, const uint32_t x);

// Allocates a cora_string initialized to x and places it into out.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the string
// couldn't be allocated.
bool cora_string(struct cora_state* state, size_t* out, const char* x);

// Allocates a cora_bool initialized to x and places it into out.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the bool
// couldn't be allocated.
bool cora_bool(struct cora_state* state, size_t* out, const bool x);

// Allocates a cora_list initialized to x and places it into out.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the bool
// couldn't be allocated.
bool cora_bool(struct cora_state* state, size_t* out, const bool x);

// Allocates an empty cora_map and places it into out.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the map
// couldn't be allocated.
bool cora_map(struct cora_state* state, size_t* out);

// Inserts the pair (name, value) into map. value must be a valid handle to an
// object.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the map
// couldn't be allocated.
bool cora_map_insert(struct cora_state* state, const size_t map,
		const char* name, const size_t value);

// Deletes the value associated with name from map. If name is not already in
// map, nothing happens.
void cora_map_del(
		struct cora_state* state, const size_t map, const char* name);

// Returns each (name, value) pair as an array of names, an array of values,
// and a length.
//
// These should not be modified.
void cora_map_pairs(struct cora_state* state, const size_t map, char*** names,
		size_t** values, size_t* len);

// Returns the length (number of key-value pairs) in the map.
size_t cora_map_length(struct cora_state* state, const size_t map);

// Allocates an empty cora_list and places it into out.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the list
// couldn't be allocated.
bool cora_list(struct cora_state* state, size_t* out);

// Appends x to the list l.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the list
// couldn't be allocated.
bool cora_list_append(struct cora_state* state, size_t* l, const size_t x);

// Inserts x to the list l at the given index.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the list
bool cora_list_insert(struct cora_state* state, size_t* l, const size_t x,
		const size_t index);

// Deletes the element in the list l at the given index.
//
// If the index is past the end of the list, nothing happens.
//
// Returns true on success and CORA_NO_MEMORY if the memory for the list
void cora_list_del(struct cora_state* state, size_t* l, const size_t x,
		const size_t index);

// Returns the length of the given cora_list.
size_t cora_list_length(struct cora_state* state, const size_t list);

// Returns a pointer to the items contained in the list, as well as the length
// of the list.
//
// It is important that these not be modified directly. Use the cora_list_*
// functions to add or remove things from the list.
void cora_list_items(struct cora_state* state, const size_t list,
		size_t* out_len, size_t* out);

#endif // CORA_H
