/*
    vector.c
    Michaiah J. Williams
    2023-11-03
    An aproximation of the c++ vector class
    First started working on this as part of a linear algebra calculator (coming soon!) as a good way to read data 
    (because I needed strings that could resize themselves and realized a more general container was useful)
    I'll try to match all as many of the functions listed on cppreference but I'm starting with .at() and .push_back()
    Obviously everything, and especially initiallization is going to look diferent, but hopefully these will be usable and generic
    Is it safe? Wouldn't assume so no. Much the opposite actually. Oh well. Expect updates.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct vector {
    void* data; //this will not be a disaster...
    int size; //next open slot (or null terminator in strings)
    int capacity;
    int data_size; //for "typing"
} vector;
//to demonstrate that vectors work with structs
typedef struct example {
    int int_num;
    float float_num;
    char* pointer_char;
    double array100 [100];
    int array_size;
} example;

//free data (very important)
void free_vector(vector v){
    free(v.data);
}

void* at(vector v, int idx){ //what else could it return
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
    char* end = at(*v, v->size); //byte by byte...
    for(int i = 0; i < v->data_size; i++){
        end[i] = ((char*)data)[i];
    } //I beleive this is what cppreference refered to as *trivially copyable*
    (v->size)++;
    return 1;
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
    printf("str contains [ %s ]\nsize is %d\ncapacity is %d\n", str.data, str.size, str.capacity);

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
    free_vector(str);
    free_vector(vec_double);
    free_vector(vec_struct);

    return 0;

}