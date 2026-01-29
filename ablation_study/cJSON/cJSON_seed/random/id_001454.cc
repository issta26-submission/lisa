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
//<ID> 1454
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
    cJSON *old_item = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *found_old = (cJSON *)0;
    cJSON *val_arr = (cJSON *)0;
    cJSON *picked0 = (cJSON *)0;
    cJSON *picked1 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Setup - create root object, an array and two numeric elements, attach array and an "old" string
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.5);
    num1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added = cJSON_AddItemToObject(root, "values", arr);
    old_item = cJSON_CreateString("replace_me");
    cJSON_AddItemToObject(root, "old", old_item);

    // step 3: Configure - locate the "old" item and replace it via pointer with a numeric node
    found_old = cJSON_GetObjectItem(root, "old");
    replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemViaPointer(root, found_old, replacement);

    // step 4: Operate - compute sum of array elements, add sum to object, and append another element to array
    val_arr = cJSON_GetObjectItem(root, "values");
    picked0 = cJSON_GetArrayItem(val_arr, 0);
    picked1 = cJSON_GetArrayItem(val_arr, 1);
    sum_val = cJSON_GetNumberValue(picked0) + cJSON_GetNumberValue(picked1);
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);
    cJSON_AddItemToArray(val_arr, cJSON_CreateNumber(3.0));

    // step 5: Validate - produce an unformatted print and derive a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)(added != 0) + (int)cJSON_IsNumber(cJSON_GetObjectItem(root, "old"));

    // step 6: Cleanup - free printed buffer and delete JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)num0;
    (void)num1;
    (void)replacement;
    (void)picked0;
    (void)picked1;
    // API sequence test completed successfully
    return 66;
}