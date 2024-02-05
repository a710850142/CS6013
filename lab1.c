
/*********************************************************************
*
* Filename: lab1.c
* Author: Xiyao Xu
* Class: CS6013
* Date: January 13, 2024
* Assignment: Lab1
*
* Description:
* This C program simulates several functionalities commonly found in
* operating systems. It includes implementations of functions like
* byte_sort() and nibble_sort() to manipulate byte and nibble order,
* and name_list() to create a linked list of characters representing a
* name. The program also contains draw_me(), a function to create an ASCII
* art representation. Each function is accompanied by its respective test
* functions to ensure proper operation. The main focus of this lab is to
* practice memory management, bitwise operations, and file handling in C.
* The code is intended for educational purposes in a systems programming
* course. The program compiles in C mode, not C++, adhering to C standards
* and practices.
*
*********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h> // For strlen()
#include <stdbool.h>
#include <assert.h>
/*********************************************************************
*
* The C functions in this lab use patterns and functionality often found in
* operating system code. Your job is to implement them. Additionally, write some
test
* cases for each function (stick them in functions [called xyzTests(), etc or
similar])
* Call your abcTests(), etc functions in main().
*
* Write your own tests for each function you are testing, then share/combine
* tests with a classmate. Try to come up with tests that will break people's
code!
* Easy tests don't catch many bugs! [Note this is one specific case where you may
* share code directly with another student. The test function(s) from other
students
* must be clearly marked (add '_initials' to the function name) and commented with
the
* other student's name.
*
* Note: you may NOT use any global variables in your solution.
*
* Errata:
* - You can use global vars in your testing functions (if necessary).
* - Don't worry about testing the free_list(), or the draw_me() functions.
*
* You must compile in C mode (not C++). If you compile from the commandline
* run clang, not clang++. Submit your solution files on Canvas.
*
*********************************************************************/
/*********************************************************************
*
* byte_sort()
*
* specification: byte_sort() treats its argument as a sequence of
* 8 bytes, and returns a new unsigned long integer containing the
* same bytes, sorted numerically, with the smaller-valued bytes in
* the lower-order byte positions of the return value
*
* EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
* Ah, the joys of using bitwise operators!
*
* Hint: you may want to write helper functions for these two functions
*
*********************************************************************/
unsigned long byte_sort(unsigned long arg) {
    unsigned long result = 0;
    for (int i = 0; i < 8; i++) {
        unsigned char min = (arg >> (i * 8)) & 0xFF;
        int min_index = i;

        for (int j = i + 1; j < 8; j++) {
            unsigned char current = (arg >> (j * 8)) & 0xFF;
            if (current < min) {
                min = current;
                min_index = j;
            }
        }

        // Swap
        if (min_index != i) {
            unsigned char current_i = (arg >> (i * 8)) & 0xFF;
            arg &= ~((unsigned long)0xFF << (min_index * 8)); // Clear the min byte
            arg |= (unsigned long)current_i << (min_index * 8); // Put i-th byte in min's place
        }

        result |= (unsigned long)min << (i * 8);
    }
    return result;
}





/*********************************************************************
*
* nibble_sort()
*
* specification: nibble_sort() treats its argument as a sequence of 16 4-bit
* numbers, and returns a new unsigned long integer containing the same nibbles,
* sorted numerically, with smaller-valued nibbles towards the "small end" of
* the unsigned long value that you return
*
* the fact that nibbles and hex digits correspond should make it easy to
* verify that your code is working correctly
*
* EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
*
*********************************************************************/
unsigned long nibble_sort(unsigned long arg) {
    unsigned long result = 0;

    for (int i = 0; i < 16; i++) {
        unsigned char key = (arg >> (i * 4)) & 0xF;

        int j = i - 1;
        while (j >= 0 && ((result >> (j * 4)) & 0xF) > key) {
            result &= ~((unsigned long)0xF << ((j + 1) * 4)); // Clear the target nibble
            result |= ((unsigned long)((result >> (j * 4)) & 0xF) << ((j + 1) * 4)); // Move j-th nibble to j+1 position
            j--;
        }
        result &= ~((unsigned long)0xF << ((j + 1) * 4)); // Clear the target nibble
        result |= (unsigned long)key << ((j + 1) * 4);
    }

    return result;
}






/*********************************************************************/
typedef struct elt {
    char val;
    struct elt *link;
} Elt;
/*********************************************************************/
/* Forward declaration of "free_list()"... This allows you to call */
/* free_list() in name_list() [if you'd like]. */
void free_list( Elt* head ); // [No code goes here!]
/*********************************************************************
*
* name_list()
*
* specification: allocate and return a pointer to a linked list of
* struct elts
*
* - the first element in the list should contain in its "val" field the first
* letter of your first name; the second element the second letter, etc.;
*
* - the last element of the linked list should contain in its "val" field
* the last letter of your first name and its "link" field should be a null
* pointer
*
* - each element must be dynamically allocated using a malloc() call
*
* - you must use the "name" variable (change it to be your name).
*
* Note, since we're using C, not C++ you can't use new/delete!
* The analog to delete is the free() function
*
* - if any call to malloc() fails, your function must return NULL and must also
* free any heap memory that has been allocated so far; that is, it must not
* leak memory when allocation fails
*
* Implement print_list and free_list which should do what you expect.
* Printing or freeing a nullptr should do nothing.
*
* Note: free_list() might be useful for error handling for name_list()...
*
*********************************************************************/
Elt *name_list() {
    char *name = "Davison";
    Elt *head = NULL, *tail = NULL;

    for (int i = 0; name[i] != '\0'; ++i) {
        Elt *newElt = (Elt *)malloc(sizeof(Elt));
        if (newElt == NULL) {
            free_list(head);
            return NULL;
        }
        newElt->val = name[i];
        newElt->link = NULL;

        if (head == NULL) {
            head = newElt;
        } else {
            tail->link = newElt;
        }
        tail = newElt;
    }

    return head;
}


/*********************************************************************/
void print_list(Elt *head) {
    Elt *current = head;
    while (current != NULL) {
        printf("%c ", current->val);
        current = current->link;
    }
    printf("\n");
}

/*********************************************************************/
void free_list(Elt *head) {
    Elt *current = head;
    while (current != NULL) {
        Elt *next = current->link;
        free(current);
        current = next;
    }
}

/*********************************************************************
*
* draw_me()
*
* This function creates a file called 'me.txt' which contains an ASCII-art
* picture of you (it does not need to be very big).
*
* Use the C stdlib functions: fopen, fclose, fprintf, etc which live in stdio.h
* - Don't use C++ iostreams
*
*********************************************************************/
void draw_me() {
    FILE *f = fopen("me.txt", "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Example ASCII art (replace with your own)
    fprintf(f, ":-)\n");

    fclose(f);
}

/*********************************************************************
*
* Test Code - Place your test functions in this section:
*/
// Helper function to check if the test passed
bool testByteSort() {
    unsigned long input = 0x0403deadbeef0201;
    unsigned long expected = 0xefdebead04030201;
    unsigned long result = byte_sort(input);

    if (result == expected) {
        printf("byte_sort passed\n");
        return true;
    } else {
        printf("byte_sort failed: expected 0x%lx, got 0x%lx\n", expected, result);
        return false;
    }
}

bool testNibbleSort() {
    unsigned long input = 0x0403deadbeef0201;
    unsigned long expected = 0xfeeeddba43210000;
    unsigned long result = nibble_sort(input);

    if (result == expected) {
        printf("nibble_sort passed\n");
        return true;
    } else {
        printf("nibble_sort failed: expected 0x%lx, got 0x%lx\n", expected, result);
        return false;
    }
}




bool testNameList() {
    Elt *list = name_list();
    if (list == NULL) {
        printf("name_list failed: returned NULL\n");
        return false;
    }

    // Assuming the name is "Davison"
    char expectedName[] = "Davison";
    Elt *current = list;
    int i = 0;
    bool failed = false;
    while (current != NULL) {
        if (current->val != expectedName[i]) {
            printf("name_list failed: expected '%c', got '%c'\n", expectedName[i], current->val);
            failed = true;
            break;
        }
        current = current->link;
        i++;
    }

    free_list(list);

    if (!failed) {
        printf("name_list, print_list, and free_list passed\n");
        return true;
    }
    return false;
}


bool testDrawMe() {
    draw_me();
    FILE *file = fopen("me.txt", "r");
    if (file) {
        printf("draw_me passed\n");
        fclose(file);
        return true;
    } else {
        printf("draw_me failed: me.txt not created\n");
        return false;
    }
}

//Ray's code
//bool TestByteSort(unsigned long testValue, unsigned long expectedValue) {
//    unsigned long sortedValue = byte_sort(testValue);
//    return sortedValue == expectedValue;
//}
//
//bool TestNibbleSort(unsigned long testValue, unsigned long expectedValue) {
//    unsigned long sortedValue = nibble_sort(testValue);
//    return sortedValue == expectedValue;
//}
/*********************************************************************
*
* main()
*
* The main driver program. You can place your main() method in this
* file to make compilation easier, or have it in a separate file.
*
*
*********************************************************************/
int main() {
    bool nameListSuccess = testNameList();
    bool drawMeSuccess = testDrawMe();
    bool byteSortSuccess = testByteSort();
    bool nibbleSortSuccess = testNibbleSort();

    //Ray's code
//    bool byte1 = testByteSort(0x0403deadbeef0201, 0xefdebead04030201);
//    bool byte2 = testByteSort(0x0405deadbeef0201, 0xefdebead05040201);
//    bool byte3 = testByteSort(0x1234567890abcdef, 0xefcdab9078563412);
//    bool byte4 = testByteSort(0x11112111111111ef, 0xef21111111111111);
//
//    if (byte1 & byte2 & byte3 & byte4) {
//        printf("All tests passed for byte_sort function!\n");
//    } else {
//        printf("Tests failed for byte_sort function!\n");
//    }
//
//
//    bool nibble1 = testNibbleSort(0x0403deadbeef0201, 0xfeeeddba43210000);
//    bool nibble2 = testNibbleSort(0x0405deadbeef0201, 0xfeeeddba54210000);
//    bool nibble3 = testNibbleSort(0x1234567890abcdef, 0xfedcba9876543210);
//    bool nibble4 = testNibbleSort(0x11112111111111ef, 0xfe21111111111111);
//
//    if (nibble1 & nibble2 & nibble3 & nibble4) {
//        printf("All tests passed for nibble_sort function!\n");
//    } else {
//        printf("Tests failed for nibble_sort function!\n");
//    }

    return 0;
}

