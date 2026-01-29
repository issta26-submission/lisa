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
//<ID> 206
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *orig = cJSON_CreateStringReference("original_value");
    cJSON_AddItemToObject(child, "name", orig);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateStringReference("replaced_value");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(child, orig, replacement);

    // step 3: Operate & Validate
    cJSON *got = cJSON_GetObjectItem(child, "name");
    const char *got_str = cJSON_GetStringValue(got);
    cJSON *copy_ref = cJSON_CreateStringReference(got_str);
    cJSON_AddItemToObject(root, "copy", copy_ref);
    cJSON *nil_item = cJSON_CreateNull();
    cJSON_bool nil_is_null = cJSON_IsNull(nil_item);
    cJSON_AddItemToObject(root, "nil", nil_item);

    // step 4: Cleanup
    (void)replaced;
    (void)nil_is_null;
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}