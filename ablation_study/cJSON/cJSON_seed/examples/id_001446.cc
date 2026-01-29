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
//<ID> 1446
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
    cJSON *null_item = cJSON_CreateNull();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *str_ref = cJSON_CreateStringReference("hello");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "child", child);
    cJSON_AddItemToObjectCS(child, "none", null_item);
    cJSON_AddItemToObjectCS(child, "flag", false_item);
    cJSON_AddItemToObjectCS(root, "greeting", str_ref);

    // step 3: Operate & Validate
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObjectCS(root, "child_copy", dup_child);
    cJSON *flag_ptr = cJSON_GetObjectItem(child, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_ptr);
    cJSON *none_ptr = cJSON_GetObjectItem(child, "none");
    cJSON_bool none_is_null = cJSON_IsNull(none_ptr);
    const char *greeting = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    (void)flag_is_false;
    (void)none_is_null;
    (void)greeting;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}