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
//<ID> 456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *count = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(meta, "count", count);

    // step 2: Configure
    const char *orig = cJSON_GetStringValue(greet);
    cJSON *copy = cJSON_CreateString(orig);
    cJSON_AddItemToObject(root, "greeting_copy", copy);
    cJSON *replacement = cJSON_CreateString("hi!");
    cJSON *orig_ptr = cJSON_GetObjectItem(root, "greeting");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, orig_ptr, replacement);
    (void)replaced;

    // step 3: Operate
    cJSON_bool was_false = cJSON_IsFalse(flag);
    cJSON_AddBoolToObject(root, "was_disabled", was_false);
    const char *newval = cJSON_GetStringValue(replacement);
    cJSON_AddStringToObject(root, "final_greeting", newval);

    // step 4: Validate and Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}