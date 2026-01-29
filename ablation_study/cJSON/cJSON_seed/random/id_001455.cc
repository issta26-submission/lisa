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
//<ID> 1455
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
    cJSON *num2 = (cJSON *)0;
    cJSON *status_num = (cJSON *)0;
    cJSON *status_str = (cJSON *)0;
    cJSON *values_ptr = (cJSON *)0;
    cJSON *status_ptr = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    char *printed = (char *)0;
    double sum = 0.0;
    int validation_score = 0;
    cJSON_bool added_values = 0;
    cJSON_bool added_status = 0;
    cJSON_bool replaced = 0;
    cJSON_bool added_array_item = 0;

    // step 2: Initialize - create root object, an array and numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.5);
    num1 = cJSON_CreateNumber(2.5);
    num2 = cJSON_CreateNumber(3.0);

    // step 3: Configure - populate array and attach to root, add a status numeric node
    added_array_item = cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added_values = cJSON_AddItemToObject(root, "values", arr);
    status_num = cJSON_CreateNumber(0);
    added_status = cJSON_AddItemToObject(root, "status", status_num);

    // step 4: Operate - retrieve pointers via GetObjectItem, append another array item, replace status node via pointer
    values_ptr = cJSON_GetObjectItem(root, "values");
    added_array_item = cJSON_AddItemToArray(values_ptr, num2);
    status_ptr = cJSON_GetObjectItem(root, "status");
    status_str = cJSON_CreateString("ok");
    replaced = cJSON_ReplaceItemViaPointer(root, status_ptr, status_str);

    // step 5: Validate - compute sum from array elements, produce printed output and derive a small validation score
    picked = cJSON_GetArrayItem(values_ptr, 1);
    sum = cJSON_GetNumberValue(cJSON_GetArrayItem(values_ptr, 0)) + cJSON_GetNumberValue(cJSON_GetArrayItem(values_ptr, 1)) + cJSON_GetNumberValue(cJSON_GetArrayItem(values_ptr, 2));
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)(replaced != 0) + (int)cJSON_IsNumber(picked);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum;
    (void)added_values;
    (void)added_status;
    (void)added_array_item;
    (void)values_ptr;
    (void)status_ptr;
    // API sequence test completed successfully
    return 66;
}