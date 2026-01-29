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
//<ID> 1092
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *s_old = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(child, "key", s_old);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);

    // step 2: Configure
    cJSON *s_new = cJSON_CreateString("new_value");
    cJSON_bool replaced_obj = cJSON_ReplaceItemInObject(child, "key", s_new);
    (void)replaced_obj;
    cJSON *retrieved = cJSON_GetObjectItem(child, "key");
    const char *retrieved_val = cJSON_GetStringValue(retrieved);
    cJSON *copy = cJSON_CreateString(retrieved_val);

    // step 3: Operate & Validate
    cJSON_AddItemToObject(root, "copied", copy);
    cJSON *enabled_ptr = cJSON_GetObjectItem(root, "enabled");
    cJSON *enabled_new = cJSON_CreateTrue();
    cJSON_bool replaced_via_ptr = cJSON_ReplaceItemViaPointer(root, enabled_ptr, enabled_new);
    (void)replaced_via_ptr;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}