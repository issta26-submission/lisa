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
//<ID> 1129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(3.14);
    cJSON *n1 = cJSON_CreateNumber(2.71);
    const char *ver = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(ver);
    cJSON *label_ref = cJSON_CreateStringReference("initial_label");

    // step 2: Configure
    cJSON_AddItemToArray(nums, n0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToObject(data, "numbers", nums);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(root, "version", ver_ref);
    cJSON_AddItemToObject(root, "label", label_ref);

    // step 3: Operate & Validate
    cJSON *nums_arr_ref = cJSON_CreateArrayReference(nums);
    cJSON_AddItemToObject(root, "numbers_ref", nums_arr_ref);
    cJSON *new_label_ref = cJSON_CreateStringReference("replaced_label");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "label", new_label_ref);
    cJSON *it0 = cJSON_GetArrayItem(nums, 0);
    cJSON *it1 = cJSON_GetArrayItem(nums, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    cJSON_AddNumberToObject(root, "sum", v0 + v1);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}