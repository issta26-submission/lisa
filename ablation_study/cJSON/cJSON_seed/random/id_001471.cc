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
//<ID> 1471
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
    cJSON *first_elem = (cJSON *)0;
    cJSON *second_elem = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int arr_size = 0;
    cJSON_bool added_array = 0;
    cJSON_bool added_n0 = 0;
    cJSON_bool added_n1 = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(20.5);

    // step 3: Configure - populate array and attach to root using AddItemToObject
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    added_array = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate - retrieve array elements, compute sum, and add sum using AddNumberToObject
    first_elem = cJSON_GetArrayItem(arr, 0);
    second_elem = cJSON_GetArrayItem(arr, 1);
    arr_size = cJSON_GetArraySize(arr);
    sum_val = cJSON_GetNumberValue(first_elem) + cJSON_GetNumberValue(second_elem);
    sum_node = cJSON_AddNumberToObject(root, "sum", sum_val);

    // step 5: Validate - remove the original array from root and produce a printed JSON
    cJSON_DeleteItemFromObject(root, "values");
    printed = cJSON_PrintUnformatted(root);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)arr_size;
    (void)added_array;
    (void)added_n0;
    (void)added_n1;
    (void)first_elem;
    (void)second_elem;
    (void)sum_node;
    (void)sum_val;
    // API sequence test completed successfully
    return 66;
}