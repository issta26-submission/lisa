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
//<ID> 1441
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
    double v0 = 0.0;
    double v1 = 0.0;
    double sum_val = 0.0;
    int validation_score = 0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool attached = 0;

    // step 2: Initialize - create root object, an array and two numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(32.0);

    // step 3: Configure - add numeric items to the array and attach the array and a descriptive string to root
    added0 = cJSON_AddItemToArray(arr, num0);
    added1 = cJSON_AddItemToArray(arr, num1);
    attached = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddStringToObject(root, "desc", "two numbers");

    // step 4: Operate - read back values from array, compute sum, create a sum node and attach it to root
    picked0 = cJSON_GetArrayItem(arr, 0);
    picked1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(picked0);
    v1 = cJSON_GetNumberValue(picked1);
    sum_val = v0 + v1;
    sum_node = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_node);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)(picked0 != (cJSON *)0) + (int)(picked1 != (cJSON *)0) + (int)(sum_node != (cJSON *)0) + (int)attached + (int)added0 + (int)added1;

    // step 6: Cleanup - delete the entire JSON tree
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)v0;
    (void)v1;
    return 66;
}