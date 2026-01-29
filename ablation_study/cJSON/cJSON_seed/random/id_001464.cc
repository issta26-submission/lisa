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
//<ID> 1464
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
    cJSON *picked = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *str_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum = 0.0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create root object, an array and three numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.5);
    n1 = cJSON_CreateNumber(4.5);
    n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    added = cJSON_AddItemToObject(root, "numbers", arr);
    str_node = cJSON_AddStringToObject(root, "label", "sample");

    // step 3: Operate - compute sum and average using number retrieval and array size
    arr_size = cJSON_GetArraySize(arr);
    picked = cJSON_GetArrayItem(arr, 0);
    sum = cJSON_GetNumberValue(picked) + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1)) + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    sum_node = cJSON_CreateNumber(sum / (double)arr_size);
    cJSON_AddItemToObject(root, "average", sum_node);

    // step 4: Validate - produce unformatted print and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)(str_node != (cJSON *)0) + (int)(added != 0) + (int)cJSON_IsNumber(sum_node);

    // step 5: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum;
    (void)arr_size;
    (void)picked;
    (void)n0;
    (void)n1;
    (void)n2;
    (void)added;
    (void)str_node;
    (void)sum_node;
    // API sequence test completed successfully
    return 66;
}