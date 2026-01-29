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
//<ID> 1125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    const char *ver = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(ver);

    cJSON_AddItemToArray(nums, n0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToObject(root, "numbers", nums);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON *nums_ref = cJSON_Duplicate(nums, 1);
    cJSON_AddItemToObject(root, "numbers_ref", nums_ref);
    cJSON *sum_placeholder = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "sum", sum_placeholder);

    cJSON *it0 = cJSON_GetArrayItem(nums, 0);
    cJSON *it1 = cJSON_GetArrayItem(nums, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double total = v0 + v1;
    cJSON *actual_sum = cJSON_CreateNumber(total);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "sum", actual_sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    cJSON_Delete(root);
    return 66;
}