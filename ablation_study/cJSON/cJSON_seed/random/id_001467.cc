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
//<ID> 1467
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
    cJSON *avg_node = (cJSON *)0;
    cJSON *picked0 = (cJSON *)0;
    cJSON *picked1 = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    double avg_val = 0.0;
    int count = 0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create root object, an array and three numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.0);
    n1 = cJSON_CreateNumber(6.0);
    n2 = cJSON_CreateNumber(9.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Configure - attach array to root and add a descriptive string property
    added = cJSON_AddItemToObject(root, "values", arr);
    cJSON *label = cJSON_AddStringToObject(root, "label", "sample_dataset");

    // step 4: Operate - compute sum and average using array size and number values, add results to root
    count = cJSON_GetArraySize(arr);
    picked0 = cJSON_GetArrayItem(arr, 0);
    picked1 = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(picked0) + cJSON_GetNumberValue(picked1) + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    avg_val = sum_val / (double)count;
    sum_node = cJSON_CreateNumber(sum_val);
    avg_node = cJSON_CreateNumber(avg_val);
    cJSON_AddItemToObject(root, "sum", sum_node);
    cJSON_AddItemToObject(root, "average", avg_node);

    // step 5: Validate - produce an unformatted JSON string and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)(added != 0) + (int)cJSON_IsNumber(sum_node) + (int)cJSON_IsNumber(avg_node);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)label;
    (void)n0;
    (void)n1;
    (void)n2;
    (void)picked0;
    (void)picked1;
    (void)count;
    (void)sum_val;
    (void)avg_val;
    // API sequence test completed successfully
    return 66;
}