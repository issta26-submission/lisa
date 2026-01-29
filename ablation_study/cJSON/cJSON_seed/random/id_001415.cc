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
//<ID> 1415
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
    cJSON *elem2 = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    cJSON *num_node = (cJSON *)0;
    char *printed = (char *)0;
    int arr_size = 0;
    int is_array = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array with three string elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateString("one");
    elem1 = cJSON_CreateString("two");
    elem2 = cJSON_CreateString("three");

    // step 3: Configure - populate the array and attach it to root
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - query array properties and add a number to the root based on array size
    arr_size = cJSON_GetArraySize(arr);
    is_array = (int)cJSON_IsArray(arr);
    second_item = cJSON_GetArrayItem(arr, 1);
    num_node = cJSON_AddNumberToObject(root, "count", (double)arr_size);
    printed = cJSON_Print(root);

    // step 5: Validate - compute a small validation score using results of operations
    validation_score = arr_size + is_array + (int)(second_item != (cJSON *)0) + (int)(num_node != (cJSON *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed string and delete the whole JSON tree
    if (printed) { cJSON_free(printed); } /* allowed single statement cleanup (no branching logic affecting flow) */
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)elem0;
    (void)elem1;
    (void)elem2;
    (void)second_item;
    return 66;
}