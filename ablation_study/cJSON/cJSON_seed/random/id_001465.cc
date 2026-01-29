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
//<ID> 1465
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
    cJSON *sum_node = (cJSON *)0;
    cJSON *picked0 = (cJSON *)0;
    cJSON *picked1 = (cJSON *)0;
    cJSON *label_node = (cJSON *)0;
    char *printed = (char *)0;
    double sum_val = 0.0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create root object, an array and two numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(12.5);
    num1 = cJSON_CreateNumber(17.5);

    // step 3: Configure - populate array and attach to root, add a descriptive string
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added = cJSON_AddItemToObject(root, "numbers", arr);
    label_node = cJSON_AddStringToObject(root, "source", "generated");

    // step 4: Operate - inspect array, compute sum using number value API and add result to root
    arr_size = cJSON_GetArraySize(arr);
    picked0 = cJSON_GetArrayItem(arr, 0);
    picked1 = cJSON_GetArrayItem(arr, 1);
    sum_val = cJSON_GetNumberValue(picked0) + cJSON_GetNumberValue(picked1);
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - produce unformatted JSON and derive a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(printed != (char *)0) + (int)(arr_size == 2) + (int)(label_node != (cJSON *)0) + (int)(added != 0);

    // step 6: Cleanup - free printed buffer and delete JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)sum_val;
    (void)picked0;
    (void)picked1;
    (void)arr_size;
    (void)added;
    (void)label_node;

    // API sequence test completed successfully
    return 66;
}