/*
    vector.c
    Michaiah J. Williams
    2023-11-03
    An aproximation of the c++ vector class
    First started working on this as part of a linear algebra calculator (coming soon!) as a good way to read data 
    (because I needed strings that could resize themselves and realized a more general container was useful)
    Obviously everything, and especially initiallization is going to look diferent, but hopefully these will be usable and generic
    Is it safe? Wouldn't assume so no. Much the opposite actually. Oh well. Expect updates.
*/

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

//to demonstrate that vectors work with structs
typedef struct example {
    int int_num;
    float float_num;
    char* pointer_char;
    double array100 [100];
    int array_size;
} example;

//free data (very important)
void free_vector(vector* v){
    free(v->data);
    v->data = NULL;
    v->capacity = 0;
    v->size = 0;
}

void* at(vector v, int idx){ //what else could it return
    if(idx < 0 || idx >= v.size){
        return NULL;
    }
    return ((char*)v.data) + idx*v.data_size;
}

int push_back_str(vector* v, char ch){
    if(v -> size >= v -> capacity - 1){
        char* new_data = realloc(v->data, 2*(v->capacity + 1)); //yes this is purposefully different
        if(new_data == NULL){
            return 0;
        }
        v->data = new_data;
        v->capacity = 2*(v->capacity + 1);
    }
    char* data = v->data;
    data[v->size] = ch;
    (v->size)++;
    data[v->size] = '\0';
    return 1;
}

//int because it returns 0 on failure 
int push_back(vector* v, void* data){ //... neither will this
    if(v -> data_size == 1){ //strings should be null-terminated and are therefore handled slightly differently
        return push_back_str(v, *((char*)data));
    }
    if(v -> size >= v -> capacity){ //reallocation required
        void* new_data = realloc(v->data, v->data_size*(2*(v->capacity) + 1)); //note realloc copies data for you
        if(new_data == NULL){ //catastrophic failure
            return 0;
        }
        v->data = new_data;
        v->capacity = 2*(v->capacity) + 1;
    }
    char* end = v->size ? at(*v, v->size - 1) + v->data_size : v->data; //byte by byte...
    for(int i = 0; i < v->data_size; i++){
        end[i] = ((char*)data)[i];
    } //I beleive this is what cppreference refered to as *trivially copyable*
    (v->size)++;
    return 1;
}

void* front(vector v){
    return v.data;
}

void* back(vector v){
    if(!v.size){
        return NULL;
    }
    if(v.data_size == 1){ //null-terminated string
        return at(v, v.size - 2);
    }
    return at(v, v.size - 1);
}

int empty(vector v){
    return !(v.size);
}

void clear(vector* v){
    v->size = 0;
}

//returns 0 on failure
int reserve(vector* v, int new_cap){
    if(v->capacity < new_cap){
        void* new_data = realloc(v->data, (v->data_size)*new_cap);
        if(new_data == NULL){
            return 0;
        }
        v->data = new_data;
        v->capacity = new_cap;
    }
    return 1;
}

//returns 0 on failure
int shrink_to_fit(vector* v){
    void* new_data = realloc(v->data, (v->data_size)*(v->size));
    if(new_data == NULL){
        return 0;
    }
    v->data = new_data;
    v->capacity = v->size;
    return 1;
}

void swap(vector* left, vector* right){
    vector temp = *left;
    *left = *right;
    *right = temp;
}

void* pop_back(vector* v){
    if(v->data_size == 1){ //again, null-terminated so it behaves differently
        ((char*)v->data)[v->size] = *((char*)at(*v, v->size - 1));
        (v->size)--;
        ((char*)v->data)[v->size] = '\0';
        return at(*v, v->size + 1);
    }
    (v->size)--;
    return at(*v, v->size);
}

//could fail due to bad alloc
//note this is analogous to definition (2) on cppreference: 
//void resize(size_type count, const value_type& value);
int resize(vector* v, int new_size, void* fill){
    if(new_size < v->size){
        v->size = new_size;
        if(v->data_size == 1){
            ((char*)v->data)[v->size] = '\0';
        }
        return 1;
    }else if(reserve(v, new_size + 1)){
        char* start = at(*v, v->size);
        for(int i = 0; i < new_size - v->size; i++){
            for(int j = 0; j < v->data_size; j++){
                start[i*(v->data_size) + j] = ((char*)fill)[j];
            }
        }
        v->size = new_size;
        if(v->data_size == 1){
            ((char*)v->data)[v->size] = '\0';
        }
        return 1;
    }
    //getting to this line implies new_size >= v->capacity >= v->size
    //the most reasonable thing to do here is fill in as much as possible
    char* start = at(*v, v->size);
    for(int i = 0; i < v->capacity - v->size; i++){
        for(int j = 0; j < v->data_size; j++){
            start[i*(v->data_size) + j] = ((char*)fill)[j];
        }
    }
    v->size = v->capacity;
    if(v->data_size == 1){
        (v->size)--;
        ((char*)v->data)[v->size] = '\0';
    }
    return new_size == v->size;
}

int main(){ //testing 

    vector str = {NULL, 0, 0, 1}; //correct initialization for strings
    vector vec_double = {NULL, 0, 0, sizeof(double)};
    vector vec_struct = {NULL, 0, 0, sizeof(example)};
    char ch1 = 'a';
    char ch2 = 'b';
    char ch3 = 'c';
    double d1 = 7.239;
    double d2 = 0.0003487;
    double d3 = 193.1;
    double d4 = 1982318;
    example ex1 = {32, 6.17, &ch1, {3, 0.017328, 3.1289, 23789.2}, 4};
    example ex2 = {111, 3.14, &ch2, {18932, 281.123891}, 2};

    //make str contain "abc"
    push_back(&str, &ch1);
    push_back(&str, &ch2);
    push_back(&str, &ch3);
    //make vec_double {7.239, 0.0003487, 193.1, 1982318}
    push_back(&vec_double, &d1);
    push_back(&vec_double, &d2);
    push_back(&vec_double, &d3);
    push_back(&vec_double, &d4);
    //make vec_struct contain ex1 followed by ex2
    push_back(&vec_struct, &ex1);
    push_back(&vec_struct, &ex2);

    //test for strings
    printf("str contains [ %s ]\nsize is %d\ncapacity is %d\n", (char*)str.data, str.size, str.capacity);
    //test with double
    printf("vec_double contains [ ");
    for(int i = 0; i < vec_double.size; i++){
        double* element = at(vec_double, i);
        printf("%f ", *element);
    }
    printf("]\nsize is %d\ncapacity is %d\n", vec_double.size, vec_double.capacity);

    //test with example struct
    printf("vec_struct contains\n");
    for(int i = 0; i < vec_struct.size; i++){
        example* element = at(vec_struct, i);
        printf("{ %d, %f, pointer to %c, [ ", element->int_num, element->float_num, *(element->pointer_char));
        for(int j = 0; j < element->array_size; j++){
            printf("%f ", (element->array100)[j]);
        }
        printf("] }\n");
    }
    printf("size is %d\ncapacity is %d\n", vec_struct.size, vec_struct.capacity);

    //free data
    free_vector(&str);
    free_vector(&vec_double);
    free_vector(&vec_struct);

    return 0;

}
