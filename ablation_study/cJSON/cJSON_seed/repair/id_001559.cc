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
//<ID> 1559
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json_text = "{\"greeting\":\"hello\",\"value\":7}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *greeting_item = cJSON_GetObjectItem(parsed, "greeting");
    cJSON *value_item = cJSON_GetObjectItem(parsed, "value");
    cJSON *greet_copy = cJSON_CreateString(greeting_item->valuestring);
    cJSON_AddItemToObjectCS(root, "greet", greet_copy);
    double val = cJSON_GetNumberValue(value_item);
    cJSON *num_item = cJSON_CreateNumber(val);
    cJSON_AddItemToObjectCS(root, "val", num_item);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON *to_replace = cJSON_GetObjectItem(root, "greet");
    cJSON_ReplaceItemViaPointer(root, to_replace, replacement);
    cJSON_bool is_str = cJSON_IsString(cJSON_GetObjectItem(root, "val"));
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)is_str);
    scratch[1] = out[0];
    scratch[2] = (char)('0' + (int)(val));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}