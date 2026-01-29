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
//<ID> 1445
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
    cJSON *picked = (cJSON *)0;
    cJSON *sum_node = (cJSON *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    int validation_score = 0;
    cJSON_bool attached = 0;

    // step 2: Initialize - create root object, an array and two numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(32.5);

    // step 3: Configure - add numbers to the array and attach array to root, add a descriptive string
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    attached = cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddStringToObject(root, "note", "generated numbers");

    // step 4: Operate - read values, create a sum node and append it to the array
    picked = cJSON_GetArrayItem(arr, 1);
    val1 = cJSON_GetNumberValue(picked);
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    sum_node = cJSON_CreateNumber(val0 + val1);
    cJSON_AddItemToArray(arr, sum_node);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)(cJSON_GetArraySize(arr) == 3) + (int)(picked != (cJSON *)0) + (int)cJSON_IsNumber(sum_node);

    // step 6: Cleanup - delete the whole JSON tree
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)attached;
    (void)val0;
    (void)val1;
    return 66;
}