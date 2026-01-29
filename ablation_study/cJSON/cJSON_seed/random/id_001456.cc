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
//<ID> 1456
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *new_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *old_item = (cJSON *)0;
    char *printed = (char *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added_flag = 0;
    cJSON_bool replaced_flag = 0;
    cJSON_bool pushed_flag = 0;

    // step 2: Initialize - create root object and an array with two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(5.0);
    num1 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Configure - attach the array to the root and add a label
    added_flag = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("test"));

    // step 4: Operate - retrieve the array, compute a sum, replace the first element, and append a new element
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    old_item = cJSON_GetArrayItem(retrieved_arr, 0);
    v0 = cJSON_GetNumberValue(old_item);
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_arr, 1));
    sum_val = v0 + v1;
    replacement = cJSON_CreateNumber(sum_val);
    replaced_flag = cJSON_ReplaceItemViaPointer(retrieved_arr, old_item, replacement);
    new_item = cJSON_CreateNumber(42.0);
    pushed_flag = cJSON_AddItemToArray(retrieved_arr, new_item);

    // step 5: Validate - print and derive a small validation score from results
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)(added_flag != 0) + (int)(replaced_flag != 0) + (int)(pushed_flag != 0) + (int)cJSON_IsNumber(cJSON_GetArrayItem(retrieved_arr, 0));

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)v0;
    (void)v1;
    (void)sum_val;
    (void)num0;
    (void)num1;
    (void)replacement;
    (void)new_item;
    (void)retrieved_arr;
    (void)old_item;
    // API sequence test completed successfully
    return 66;
}