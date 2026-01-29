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
//<ID> 415
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
    cJSON *newn = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    cJSON *got_sum = (cJSON *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;
    double validated = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(20.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    label = cJSON_AddStringToObject(root, "description", "sample numbers");

    // step 4: Operate
    newn = cJSON_CreateNumber(99.5);
    cJSON_ReplaceItemInArray(arr, 1, newn);
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    sum = v0 + v1;
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 5: Validate
    got_sum = cJSON_GetObjectItem(root, "sum");
    validated = cJSON_GetNumberValue(got_sum);
    (void)validated;
    (void)label;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}