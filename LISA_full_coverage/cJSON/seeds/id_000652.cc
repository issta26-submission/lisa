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
//<ID> 652
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *num_a = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num_a);
    const char *text = "ref_text";
    cJSON *sref = cJSON_CreateStringReference(text);
    cJSON_AddItemToArray(arr, sref);
    cJSON *num_b = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num_b);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddNumberToObject(child, "val", 42.0);
    cJSON *child_val = cJSON_GetObjectItem(child, "val");

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_num = cJSON_IsNumber(first);
    (void)first_is_num;
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_invalid = cJSON_IsInvalid(second);
    (void)second_is_invalid;
    cJSON *detached = cJSON_DetachItemViaPointer(child, child_val);

    // step 4: Validate & Cleanup
    cJSON_bool detached_is_num = cJSON_IsNumber(detached);
    (void)detached_is_num;
    cJSON_Delete(detached);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}