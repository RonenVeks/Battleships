#include "IO_Assist.h"

bool 
check_key_validation(int* keys, int amount, int checked) {
    for (int k = 0; k < amount; k++)
        if (keys[k] == checked) 
            return true;
    return false;
}
