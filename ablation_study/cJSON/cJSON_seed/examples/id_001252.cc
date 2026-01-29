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
//<ID> 1252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *name_text = "alpha";
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10);
    cJSON *n1 = cJSON_CreateNumber(20);
    cJSON *n2 = cJSON_CreateNumber(30);
    cJSON_AddItemToArray(nums, n0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemToObject(root, "nums", nums);
    cJSON_AddNumberToObject(root, "mult", 3);
    cJSON_AddStringToObject(root, "name", name_text);
    cJSON *name_ref = cJSON_CreateStringReference(name_text);
    cJSON_AddItemToObject(root, "name_ref", name_ref);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(dup, "name");
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddItemToObject(sub, "detached_name", detached);

    // step 3: Operate & Validate
    cJSON *nums_from_root = cJSON_GetObjectItem(root, "nums");
    cJSON *first_item = cJSON_GetArrayItem(nums_from_root, 0);
    cJSON *second_item = cJSON_GetArrayItem(nums_from_root, 1);
    double first_val = cJSON_GetNumberValue(first_item);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON *mult = cJSON_GetObjectItem(root, "mult");
    double factor = cJSON_GetNumberValue(mult);
    double computed = (first_val + second_val) * factor;
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "%f", computed);
    (void)len;
    cJSON_AddStringToObject(dup, "computed", buf);
    cJSON_bool nums_is_array = cJSON_IsArray(nums_from_root);
    cJSON_AddBoolToObject(dup, "nums_is_array", nums_is_array);
    char *out = cJSON_PrintUnformatted(dup);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(sub);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}