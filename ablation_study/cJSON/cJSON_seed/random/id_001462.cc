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
//<ID> 1462
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
    cJSON *n2 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *title_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create root object and an array with three numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.5);
    n1 = cJSON_CreateNumber(2.5);
    n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Configure - attach array to root and add a descriptive string property
    added = cJSON_AddItemToObject(root, "values", arr);
    title_node = cJSON_AddStringToObject(root, "title", "sample_numbers");

    // step 4: Operate - determine array size and compute sum of elements using number accessors
    arr_size = cJSON_GetArraySize(arr);
    sum_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0)) + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1)) + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - produce an unformatted JSON string and derive a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)(arr_size == 3) + (int)(title_node != (cJSON *)0) + (int)(added != 0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)n0;
    (void)n1;
    (void)n2;
    (void)arr_size;
    (void)added;
    (void)title_node;
    (void)sum_node;
    // API sequence test completed successfully
    return 66;
}