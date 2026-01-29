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
//<ID> 1417
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
    cJSON *item0 = (cJSON *)0;
    cJSON *num_node = (cJSON *)0;
    char *json_str = (char *)0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool is_array = 0;

    // step 2: Initialize - create root object and an array with two string elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateString("one");
    elem1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate - inspect array, get its size and first element, and add the size as a number to root
    is_array = cJSON_IsArray(arr);
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    num_node = cJSON_AddNumberToObject(root, "count", (double)arr_size);

    // step 4: Print - produce a formatted JSON string of the whole tree
    json_str = cJSON_Print(root);

    // step 5: Validate - derive a small validation score from the operations
    validation_score = arr_size + (int)is_array + (int)(item0 != (cJSON *)0) + (int)(num_node != (cJSON *)0) + (int)(json_str != (char *)0);

    // step 6: Cleanup - free printed string and delete the tree
    cJSON_free((void *)json_str);
    cJSON_Delete(root);

    (void)validation_score;
    (void)elem0;
    (void)elem1;
    (void)item0;
    (void)arr_size;

    // API sequence test completed successfully
    return 66;
}