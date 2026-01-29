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
//<ID> 1478
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
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *temp_num = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    double temp_val = 0.0;
    cJSON_bool added_values = 0;
    cJSON_bool added_sum = 0;

    // step 2: Initialize - create root object, array and two numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(20.0);

    // step 3: Configure - populate array and attach to root; add a temporary numeric via AddNumberToObject
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    added_values = cJSON_AddItemToObject(root, "values", arr);
    temp_num = cJSON_AddNumberToObject(root, "temp", 99.0);

    // step 4: Operate - retrieve array elements, compute sum, create sum node and attach to root
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(elem0) + cJSON_GetNumberValue(elem1);
    sum_node = cJSON_CreateNumber(sum_val);
    added_sum = cJSON_AddItemToObject(root, "sum", sum_node);
    temp_val = cJSON_GetNumberValue(temp_num);

    // step 5: Validate - print unformatted JSON and derive a lightweight validation indicator
    printed = cJSON_PrintUnformatted(root);
    (void)printed;
    (void)added_values;
    (void)added_sum;
    (void)temp_val;
    (void)sum_val;

    // step 6: Cleanup - remove the temporary item, free printed buffer and delete the JSON tree
    cJSON_DeleteItemFromObject(root, "temp");
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)elem0;
    (void)elem1;
    (void)n0;
    (void)n1;
    (void)sum_node;
    (void)temp_num;
    // API sequence test completed successfully
    return 66;
}