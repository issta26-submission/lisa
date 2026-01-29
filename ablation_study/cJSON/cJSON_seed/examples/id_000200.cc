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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(info, "name", name);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(info, "maybe", maybe_null);

    // step 2: Configure
    cJSON *str_ref = cJSON_CreateStringReference("referenced_value");
    cJSON *target = cJSON_GetObjectItem(info, "name");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(info, target, str_ref);
    cJSON_bool is_null_flag = cJSON_IsNull(maybe_null);
    cJSON *null_status = cJSON_CreateBool(is_null_flag);
    cJSON_AddItemToObject(root, "maybe_was_null", null_status);

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool same = cJSON_Compare(root, dup, 1);
    cJSON *same_num = cJSON_CreateNumber((double)same);
    cJSON_AddItemToObject(root, "is_equal_to_duplicate", same_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}