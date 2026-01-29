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
//<ID> 1557
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "config", config);
    const char *json_text = "{\"name\":\"device\",\"version\":2}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON_AddItemToObjectCS(config, "parsed", parsed);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    cJSON *version_item = cJSON_GetObjectItemCaseSensitive(parsed, "version");

    // step 2: Configure
    cJSON *new_name = cJSON_CreateStringReference("fuzzed_device");
    cJSON_ReplaceItemViaPointer(parsed, name_item, new_name);

    // step 3: Operate and Validate
    cJSON_bool is_str = cJSON_IsString(new_name);
    cJSON *name_is_string = cJSON_CreateBool(is_str);
    cJSON_AddItemToObjectCS(root, "name_is_string", name_is_string);
    double ver = cJSON_GetNumberValue(version_item);
    cJSON *next_ver = cJSON_CreateNumber(ver + (double)is_str);
    cJSON_AddItemToObjectCS(config, "next_version", next_ver);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (int)ver);
    scratch[2] = (char)('0' + (int)(ver + (double)is_str));
    scratch[3] = (char)('0' + (int)(is_str != 0));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}