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
//<ID> 1209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *first_in_array = (cJSON *)0;
    int array_size = 0;
    double number_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array to hold numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Configure - create and add a string member, a true flag, and a numeric item into the array
    str_item = cJSON_CreateString("fuzz_name");
    cJSON_AddItemToObject(root, "name", str_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate - obtain array size and read the numeric value from the first element
    array_size = cJSON_GetArraySize(arr);
    first_in_array = cJSON_GetArrayItem(arr, 0);
    number_value = cJSON_GetNumberValue(first_in_array);

    // step 5: Validate - compute a small validation score from meaningful data flow
    validation_score = array_size + (int)(number_value == 42.0) + (int)(str_item != (cJSON *)0) + (int)(true_item != (cJSON *)0);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}