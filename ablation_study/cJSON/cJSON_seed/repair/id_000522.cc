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
//<ID> 522
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "key", orig);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    cJSON *key_item = cJSON_GetObjectItemCaseSensitive(root, "key");
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_bool replace_ok = cJSON_ReplaceItemViaPointer(root, key_item, replacement);
    (void)replace_ok;

    // step 3: Operate
    cJSON *key_after = cJSON_GetObjectItemCaseSensitive(root, "key");
    const char *key_str = cJSON_GetStringValue(key_after);
    cJSON *echo = cJSON_CreateString(key_str);
    cJSON_AddItemToObject(root, "echo", echo);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *echo_after = cJSON_GetObjectItemCaseSensitive(root, "echo");
    const char *echo_str = cJSON_GetStringValue(echo_after);
    (void)echo_str;
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}