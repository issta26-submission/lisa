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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(meta, "flags", flags);
    cJSON *t1 = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, t1);
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *t2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, t2);
    cJSON *detached = cJSON_DetachItemViaPointer(flags, t1);
    cJSON_AddItemToObject(meta, "detached_true", detached);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_AddBoolToObject(root, "meta_is_object", meta_is_obj);
    cJSON *dup_flags = cJSON_Duplicate(flags, 1);
    cJSON_AddItemToObject(root, "flags_copy", dup_flags);

    // step 3: Operate and Validate
    int flags_count = cJSON_GetArraySize(flags);
    cJSON_AddNumberToObject(root, "flags_count", (double)flags_count);
    cJSON *first_flag = cJSON_GetArrayItem(flags, 0);
    const char *first_flag_str = cJSON_GetStringValue(first_flag);
    cJSON_AddStringToObject(root, "first_flag_string", first_flag_str ? first_flag_str : "true");
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}