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
//<ID> 1552
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"greeting\":\"hello\",\"value\":123}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *to_replace = cJSON_CreateString("to_be_replaced");
    cJSON_AddItemToObjectCS(root, "replace_me", to_replace);
    cJSON *case_item = cJSON_CreateString("CaseSensitiveValue");
    cJSON_AddItemToObjectCS(root, "CaseKey", case_item);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.5);
    cJSON_ReplaceItemViaPointer(root, to_replace, replacement);
    cJSON *extra = cJSON_CreateString("extra_added");
    cJSON_AddItemToObjectCS(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON *greet = cJSON_GetObjectItem(root, "greeting");
    cJSON_bool is_greet_string = cJSON_IsString(greet);
    cJSON_AddBoolToObject(root, "greeting_is_string", is_greet_string);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}