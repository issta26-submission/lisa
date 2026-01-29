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
//<ID> 1205
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
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *arr_elem1 = (cJSON *)0;
    cJSON *arr_elem2 = (cJSON *)0;
    double num_value = 0.0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and child items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(3.14);
    str_item = cJSON_CreateString("hello");
    true_item = cJSON_CreateTrue();
    arr_elem1 = cJSON_CreateString("one");
    arr_elem2 = cJSON_CreateString("two");

    // step 3: Configure - assemble the JSON structure
    cJSON_AddItemToArray(arr, arr_elem1);
    cJSON_AddItemToArray(arr, arr_elem2);
    cJSON_AddItemToObject(root, "pi", num_item);
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate - retrieve numeric value and array size
    num_value = cJSON_GetNumberValue(num_item);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - produce a small validation score from retrieved values
    validation_score = (int)num_value + arr_size;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}