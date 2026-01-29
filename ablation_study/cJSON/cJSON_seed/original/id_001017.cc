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
//<ID> 1017
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"key\":null,\"flag\":false,\"text\":\"hello\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *key_item = cJSON_GetObjectItem(root, "key");
    cJSON_bool key_is_null = cJSON_IsNull(key_item);
    cJSON_AddBoolToObject(root, "key_was_null", key_is_null);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddStringToObject(nested, "inner_text", "inner");
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Operate
    cJSON *text_item = cJSON_GetObjectItem(root, "text");
    char *text_val = cJSON_GetStringValue(text_item);
    cJSON *copied = cJSON_CreateString(text_val);
    cJSON_AddItemToObject(root, "copy_text", copied);
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(root, print_buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "flag");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}