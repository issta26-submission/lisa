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
//<ID> 1693
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
    cJSON *replacement = (cJSON *)0;
    cJSON *target = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    int array_size = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.0);
    n1 = cJSON_CreateNumber(2.0);
    n2 = cJSON_CreateNumber(3.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    target = cJSON_GetArrayItem(arr, 1);
    replacement = cJSON_CreateNumber(20.0);
    cJSON_ReplaceItemViaPointer(arr, target, replacement);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(item0);
    v1 = cJSON_GetNumberValue(item1);

    // step 5: Validate
    validation = (int)(array_size == 3) + (int)(v0 == 1.0) + (int)(v1 == 20.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)n0;
    (void)n1;
    (void)n2;
    (void)replacement;
    (void)target;
    (void)item0;
    (void)item1;
    (void)array_size;
    (void)v0;
    (void)v1;
    // API sequence test completed successfully
    return 66;
}