#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1750
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *numbersArray = (cJSON *)0;
    cJSON *numberItem = (cJSON *)0;
    cJSON *detached_numbers = (cJSON *)0;
    int ints[4];
    int arr_size = 0;
    double num_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    ints[0] = 10;
    ints[1] = 20;
    ints[2] = 30;
    ints[3] = 40;
    numbersArray = cJSON_CreateIntArray((const int *)ints, 4);
    numberItem = cJSON_CreateNumber(42.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", numbersArray);
    cJSON_AddItemToObject(root, "answer", numberItem);

    // step 4: Operate
    detached_numbers = cJSON_DetachItemFromObject(root, "numbers");
    arr_size = cJSON_GetArraySize(detached_numbers);
    num_value = cJSON_GetNumberValue(numberItem);

    // step 5: Validate
    validation = (int)(arr_size == 4) + (int)(num_value == 42.5);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_numbers);
    (void)root;
    (void)numbersArray;
    (void)numberItem;
    (void)detached_numbers;
    (void)ints;
    (void)arr_size;
    (void)num_value;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}