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
//<ID> 1558
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json_text = "{\"parsed\":\"yes\",\"num\":7}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON_AddItemToObjectCS(root, "parsed_obj", parsed);
    cJSON *orig = cJSON_CreateString("original_name");
    cJSON_AddItemToObjectCS(root, "name", orig);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced_name");
    cJSON_ReplaceItemViaPointer(root, orig, replacement);
    cJSON *extra = cJSON_CreateString("extra_value");
    cJSON_AddItemToObjectCS(root, "extra", extra);

    // step 3: Operate
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "parsed");
    cJSON_bool is_str = cJSON_IsString(parsed_flag);
    const char *parsed_value = cJSON_GetStringValue(parsed_flag);
    char first_ch = parsed_value ? parsed_value[0] : ' ';
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = is_str ? '1' : '0';
    scratch[1] = (char)('0' + (int)(out_len % 10));
    scratch[2] = first_ch;

    // step 4: Validate and Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}