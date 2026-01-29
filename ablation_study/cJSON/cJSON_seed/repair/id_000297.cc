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
//<ID> 297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"flag\":true,\"name\":\"original\",\"nested\":{\"key\":\"value\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *nested = cJSON_GetObjectItem(root, "nested");
    cJSON *old_key = cJSON_GetObjectItem(nested, "key");
    cJSON *new_key = cJSON_CreateString("new_value");
    cJSON_ReplaceItemViaPointer(nested, old_key, new_key);
    cJSON_AddNumberToObject(root, "initialized_marker", 1.0);

    // step 3: Operate and Validate
    cJSON_bool nested_is_obj = cJSON_IsObject(nested);
    cJSON_AddNumberToObject(root, "nested_is_object", (double)nested_is_obj);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_true = cJSON_IsTrue(flag_item);
    cJSON_AddNumberToObject(root, "flag_numeric", (double)flag_true);
    char *key_val = cJSON_GetStringValue(cJSON_GetObjectItem(nested, "key"));
    cJSON_AddStringToObject(root, "key_copy", key_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}