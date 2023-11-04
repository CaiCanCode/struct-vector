/*

    vector.h
    Michaiah J. Williams
    2023-11-03
    heder file for vector.c

    An aproximation of the c++ vector class
    First started working on this as part of a linear algebra calculator (coming soon!) as a good way to read data 
    (because I needed strings that could resize themselves and realized a more general container was useful)
    Obviously everything, and especially initiallization is going to look diferent, but hopefully these will be usable and generic
    Is it safe? Wouldn't assume so no. Much the opposite actually. Oh well. Expect updates.

    listed public member functions of std::vector on cppreference yet to be implimented:
        - assign
        - assign_range
        - insert
        - insert_range
        - erase
        - append_range
    only includes functions that are
        a) non trivial (for example, data and capacity can be accessed directly)
        b) reasonably doable (so probably not iterators)

*/

/*

    vector OR struct vector

    vector is a struture type containing the following members:

        void* data;
            the underlying array, given by realloc()
            can safely store capacity*data_size bytes if implemented correctly
        int size;
            the number of elements being stored
            which is also the idex of the next free space
            or, in a string,
            the index of the null terminator
        int capacity;
            the maximum number of elements that can be stored without reallocation
        int data_size;
            the size in bytes of each element (1 for char, usually given by sizeof())
            used to make vector somewhat generic, though the notation is awkward

    vector is designed to approximate std::vector in C++ to the best of my ability (and also std::string because why not)

    variables of type vector should be initialized as follows 
    (assume desired_type is the type that you want the array elements to be interpreted as)

        vector name = { NULL, 0, 0, sizeof(desired_type)};
    OR
        struct vector name = { NULL, 0, 0, sizeof(desired_type)};

    this is to avoid allocation issues.
    In the future I may write a simple function to either set this default or correctly initialize to a different given initial value
    setting a vector equal to another vector is also valid, but they will share elements so may change unexpectedly

    note: ALL VECTORS SHOULD BE FREED preferably by the provided function free_vector()

*/

typedef struct vector {
    void* data;
    int size; 
    int capacity;
    int data_size; 
} vector;

/*

    free_vector()

    EXTREMELY IMPORTANT!

    purpose:
        free dynamic memory used for given vector and reset the vector to it's defaults
        defaults are 
            v->data = NULL;
            v->size = 0;
            v->capacity = 0;
        v->data_size remains unchanged (and should, in principal, never be changed)

    parameters:
        vector* v
            a pointer to the vector to be freed

    returns:
        nothing

*/
void free_vector(vector* v);

/*

    void* at(vector v, int idx);

    purpose:
        to access an element at a specific index in a vector

    paramenters:
        vector v
            the vector
        ind idx
            the index
    returns:
        a (void) pointer to the desired element 
        or, if idx is out of range,
        a null pointer

*/

void* at(vector v, int idx);

/*

    int push_back(vector* v, void* data);

    purpose:
        to append given data to the end of v->data
        allocates memory if necessary
        inserts the data, byte by byte, into the next open slot
        increments v->size

    parameters:
        vector* v
            a pointer to the vector being appended to
        void* data
            a pointer to the data to be appended
    
    returns:
        1 if successful
        0 if allocation fails

    DOES NOT CHANGE VECTOR IF ALLOCATION FAILS

*/

int push_back(vector* v, void* data);

/*

    void* front(vector v);

    purpose:
        to access the first element of v.data
    
    parameters:
        vector v
            the vector
    
    returns:
        a (void) pointer to the first element of v.data

*/

void* front(vector v);

/*

    void* back(vector v);

    purpose:
        to access the last element of v.data
    
    parameters:
        vector v
            the vector
    
    returns:
        a (void) pointer to the last element of v.data
        or, if v.size == 0,
        a null pointer
        
*/

void* back(vector v);

/*

    int empty(vector v);

    purpose:
        to determine wether a vector is empty
    
    parameters:
        vector v
            the vector
    
    returns:
        1 if v.size == 0
        0 otherwise

*/

int empty(vector v);

/*

    void clear(vector* v);

    purpose:
        to make a vector empty
        sets v->size to 0

    parameters:
        vector* v
            the vector
    returns:
        nothing

*/

void clear(vector* v);

/*

    int reserve(vector* v, int new_cap);

    purpose:
        to allocate space for a specified number of elements
        does nothing if the current capacity is greater than or equal to the requested capacity

    parameters:
        vector* v
            a pointer to the vector in question
        int new_cap
            the requested capacity

    returns:
        1 if successful
        0 if allocation fails
    
    DOES NOT CHANGE VECTOR IF ALLOCATION FAILS

*/

int reserve(vector* v, int new_cap);

/*

    int shrink_to_fit(vector* v);

    purpose:
        to shrink the capacity of a vector to it's current size
    
    parameters:
        vector* v
            a pointer to the vector in question
    
    returns:
        1 if successful
        0 if allocation fails

    DOES NOT CHANGE VECTOR IF ALLOCATION FAILS

*/

int shrink_to_fit(vector* v);

/*

    void swap(vector* left, vector* right);

    pupose:
        swap the content of 2 vectors without copying their individual elements

    parameters:
        vector* left
            a pointer to one of the vectors
        vector* right
            a pointer to the other vector

    returns:
        nothing

*/

void swap(vector* left, vector* right);

/*

    void* pop_back(vector* v);

    purpose:
        to remove and return the last elements of a vector
        decrements the size
        replaces the null terminator, if string
    
    parameters:
        vector* v
            a pointer to the vector in question

    returns:
        a (void) pointer to the removed element

*/

void* pop_back(vector* v);

/*

    int resize(vector* v, int new_size, void* fill);

    purpose:
        if the requested size is less than or equal to the current size
            truncate the vector to the requested size 
        otherwise
            fill all newly empty slots with copies of the value stored at data
        replaces the null terminator if string
        note this is similar to definition (2) on cppreference: 
        void resize(size_type count, const value_type& value);

    parameters:
        vector* v
            a pointer to the vector in question
        int new_size
            the requested size to be truncated/expanded to
        void* fill
            the data to be copied into any newly available indexes

    returns:
        1 if successful 
        0 if allocation error

    ON FAILURE:
        on failure resize() fills all available slots with the requested value, but fails to expand to the requested size

*/

int resize(vector* v, int new_size, void* fill);