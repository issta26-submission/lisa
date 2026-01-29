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
//<ID> 1257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemToArray(nums, n3);
    const char *ref_text = "original_ref";
    cJSON *ref_item = cJSON_CreateStringReference(ref_text);
    cJSON_AddItemToObject(root, "nums", nums);
    cJSON_AddItemToObject(root, "original_ref", ref_item);

    // step 2: Configure
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddStringToObject(root, "mode", "fast");

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(nums, 1);
    double second_val = cJSON_GetNumberValue(second);
    double computed_val = second_val * 2.0;
    cJSON_AddNumberToObject(root, "computed", computed_val);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "original_ref");
    cJSON *new_ref = cJSON_CreateStringReference(ref_text);
    cJSON_AddItemToObject(root, "replaced_ref", new_ref);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}