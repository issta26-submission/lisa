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
//<ID> 1391
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
    cJSON *n3 = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *added_number = (cJSON *)0;
    double extracted_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and numerical elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.5);
    n2 = cJSON_CreateNumber(2.5);
    n3 = cJSON_CreateNumber(3.5);

    // step 3: Configure - assemble the JSON tree: attach numbers to array and array to root
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate - pick the second array item, detach it from its parent, and add a number to root based on it
    picked = cJSON_GetArrayItem(arr, 1);
    extracted_value = picked->valuedouble;
    detached = cJSON_DetachItemViaPointer(arr, picked);
    added_number = cJSON_AddNumberToObject(root, "detached_value_copy", extracted_value);

    // step 5: Validate - simple arithmetic to exercise the extracted value and created nodes
    validation_score = (int)(extracted_value > 0.0) + (int)(added_number != (cJSON *)0) + (int)(detached != (cJSON *)0);

    // step 6: Cleanup - delete the root (which frees its subtree) and separately delete the detached item
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    (void)validation_score;
    (void)picked;
    (void)n1;
    (void)n2;
    (void)n3;
    (void)added_number;
    return 66;
}