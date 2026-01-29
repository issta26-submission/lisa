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
//<ID> 417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"numbers\": [1.0, 2.5, 4.0]}";
    cJSON *root = cJSON_Parse(json);
    cJSON *orig_nums = cJSON_GetObjectItem(root, "numbers");
    cJSON *orig_n0 = cJSON_GetArrayItem(orig_nums, 0);
    cJSON *orig_n1 = cJSON_GetArrayItem(orig_nums, 1);
    cJSON *orig_n2 = cJSON_GetArrayItem(orig_nums, 2);
    double orig_v0 = cJSON_GetNumberValue(orig_n0);
    double orig_v1 = cJSON_GetNumberValue(orig_n1);
    double orig_v2 = cJSON_GetNumberValue(orig_n2);

    // step 2: Configure
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(orig_v0);
    cJSON *n1 = cJSON_CreateNumber(orig_v1);
    cJSON *n2 = cJSON_CreateNumber(orig_v2);
    cJSON_AddItemToArray(new_arr, n0);
    cJSON_AddItemToArray(new_arr, n1);
    cJSON_AddItemToArray(new_arr, n2);
    double updated_n1 = cJSON_SetNumberHelper(n1, orig_v1 * 2.0);

    // step 3: Operate
    cJSON_AddItemToObject(root, "new_numbers", new_arr);
    double sum_original = orig_v0 + orig_v1 + orig_v2;
    double sum_new = cJSON_GetNumberValue(cJSON_GetArrayItem(new_arr, 0)) + cJSON_GetNumberValue(cJSON_GetArrayItem(new_arr, 1)) + cJSON_GetNumberValue(cJSON_GetArrayItem(new_arr, 2));
    (void)updated_n1;

    // step 4: Validate and Cleanup
    cJSON_AddNumberToObject(root, "sum_original", sum_original);
    cJSON_AddNumberToObject(root, "sum_new", sum_new);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}