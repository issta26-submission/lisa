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
//<ID> 908
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
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "maybe_null", maybe_null);
    cJSON *to_replace = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(meta, "replaceme", to_replace);
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "num", num_item);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("new_value");
    cJSON_bool replaced_flag = cJSON_ReplaceItemViaPointer(meta, to_replace, replacement);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced_flag);
    cJSON_bool null_flag = cJSON_IsNull(maybe_null);
    cJSON_AddNumberToObject(root, "is_null_flag", (double)null_flag);
    char *set_ret = cJSON_SetValuestring(replacement, "updated_value");
    (void)set_ret;
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_AddNumberToObject(root, "meta_is_object", (double)meta_is_obj);

    // step 3: Operate and Validate
    const char *updated_str = cJSON_GetStringValue(replacement);
    double updated_len = (double)strlen(updated_str);
    cJSON_AddNumberToObject(root, "updated_len", updated_len);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}