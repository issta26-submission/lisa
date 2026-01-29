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
//<ID> 1479
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added_array = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(12.0);
    n1 = cJSON_CreateNumber(8.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    added_array = cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate - inspect array elements, compute sum, and add sum to root using AddNumberToObject
    elem0 = cJSON_GetArrayItem(arr, 0);
    elem1 = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(elem0) + cJSON_GetNumberValue(elem1);
    sum_node = cJSON_AddNumberToObject(root, "total", sum_val);

    // step 4: Validate - print unformatted JSON and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)added_array + (int)cJSON_IsNumber(sum_node);

    // step 5: Cleanup - remove the items array from root, free printed buffer, and delete the remaining JSON tree
    cJSON_DeleteItemFromObject(root, "items");
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)elem0;
    (void)elem1;
    (void)n0;
    (void)n1;
    (void)added_array;
    // API sequence test completed successfully
    return 66;
}