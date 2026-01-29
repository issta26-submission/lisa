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
//<ID> 1202
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
    cJSON *num_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    int arr_size = 0;
    double num_val = 0.0;
    int validation_score = 0;
    cJSON_bool add_result = 0;

    // step 2: Initialize - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create items and populate the array, then attach the array to the root object
    num_item = cJSON_CreateNumber(3.14159);
    true_item = cJSON_CreateTrue();
    str_item = cJSON_CreateString("fuzz_string");
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, str_item);
    add_result = cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate - read back numeric value and array size
    num_val = cJSON_GetNumberValue(num_item);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a small validation score using obtained values
    validation_score = arr_size + (int)(num_val > 0.0) + (int)add_result;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}