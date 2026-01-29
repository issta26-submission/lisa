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
//<ID> 1556
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *input_json = "{\"greeting\":\"hello\",\"number\":123}";
    cJSON *root = cJSON_Parse(input_json);
    cJSON *extra = cJSON_CreateObject();
    cJSON *name_str = cJSON_CreateString("tester");
    cJSON_AddItemToObjectCS(extra, "name", name_str);
    cJSON_AddItemToObjectCS(root, "meta", extra);

    // step 2: Configure
    cJSON *greeting = cJSON_GetObjectItem(root, "greeting");
    cJSON *replacement = cJSON_CreateString("hi");
    cJSON_ReplaceItemViaPointer(root, greeting, replacement);
    cJSON_AddItemToObjectCS(root, "status", cJSON_CreateString("configured"));

    // step 3: Operate and Validate
    cJSON *greet_after = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    cJSON_bool is_str = cJSON_IsString(greet_after);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)is_str);
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('0' + (int)cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number")));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}