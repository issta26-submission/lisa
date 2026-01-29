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
//<ID> 1410
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    cJSON *num_node = (cJSON *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    cJSON_bool is_array = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with two string elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateString("one");
    elem1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate - inspect array, pick an element, record size as a number in root, and serialize
    arr_size = cJSON_GetArraySize(arr);
    is_array = cJSON_IsArray(arr);
    picked = cJSON_GetArrayItem(arr, 1);
    num_node = cJSON_AddNumberToObject(root, "recorded_size", (double)arr_size);
    printed = cJSON_Print(root);

    // step 4: Validate - produce a compact validation score from results
    validation_score = arr_size + (int)(is_array) + (int)(picked != (cJSON *)0) + (int)(num_node != (cJSON *)0) + (int)(printed != (char *)0);

    // step 5: Cleanup - free serialized string and delete the whole JSON tree
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)elem0;
    (void)elem1;
    (void)picked;
    return 66;
}