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
//<ID> 1444
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
    cJSON *picked = (cJSON *)0;
    cJSON *orphan = (cJSON *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    int validation_score = 0;
    cJSON_bool added = 0;

    // step 2: Initialize - create root object, an array, and numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.5);

    // step 3: Configure - add numeric elements to array, attach array to root, and add a descriptive string
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added = cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddStringToObject(root, "desc", "two numbers");

    // step 4: Operate - retrieve items, compute their sum, create a sum node and attach it
    picked = cJSON_GetArrayItem(arr, 0);
    v0 = cJSON_GetNumberValue(picked);
    picked = cJSON_GetArrayItem(arr, 1);
    v1 = cJSON_GetNumberValue(picked);
    sum_node = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum_node);

    // create and immediately delete an orphan node to exercise cJSON_Delete
    orphan = cJSON_CreateNumber(3.14);
    cJSON_Delete(orphan);

    // step 5: Validate - derive a small validation score from results (no branching)
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(sum_node != (cJSON *)0) + (int)(added != 0);

    // step 6: Cleanup - delete the entire JSON tree
    cJSON_Delete(root);

    (void)validation_score;
    (void)v0;
    (void)v1;
    (void)num0;
    (void)num1;
    (void)picked;
    // API sequence test completed successfully
    return 66;
}