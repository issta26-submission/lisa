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
//<ID> 1460
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    cJSON *picked0 = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create root object and numeric elements inside an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(3.5);
    n2 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Configure - attach array to root and add a descriptive string property
    added = cJSON_AddItemToObject(root, "numbers", arr);
    label = cJSON_AddStringToObject(root, "label", "simple-list");

    // step 4: Operate - compute sum from array elements using number retrieval and get array size
    arr_size = cJSON_GetArraySize(arr);
    picked0 = cJSON_GetArrayItem(arr, 0);
    sum_val = cJSON_GetNumberValue(picked0) + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - produce an unformatted print and derive a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = arr_size + (int)(printed != (char *)0) + (int)cJSON_IsNumber(sum_node) + (int)(label != (cJSON *)0) + (int)added;

    // step 6: Cleanup - free printed buffer and delete JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)picked0;
    (void)arr_size;
    (void)added;
    (void)label;
    // API sequence test completed successfully
    return 66;
}