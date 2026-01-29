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
//<ID> 1348
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;
    int size = 0;
    int validation_score = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_count = 0;

    // step 2: Initialize - create root object, an array and two numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.5);
    n2 = cJSON_CreateNumber(2.5);

    // step 3: Configure - place numbers into the array and attach to root using AddItemToObject
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    added_count = cJSON_AddItemToObject(root, "count", cJSON_CreateNumber(2.0));

    // step 4: Operate - retrieve the array and extract numeric values, compute sum and record size
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    elem1 = cJSON_GetArrayItem(retrieved_arr, 1);
    v0 = cJSON_GetNumberValue(elem0);
    v1 = cJSON_GetNumberValue(elem1);
    sum = v0 + v1;
    size = cJSON_GetArraySize(retrieved_arr);
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(sum));

    // step 5: Validate - derive a small validation score from operations
    validation_score = (int)added_arr + (int)added_count + (int)(size == 2) + (int)(sum == 13.0);

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)v0;
    (void)v1;
    (void)elem0;
    (void)elem1;
    return 66;
}