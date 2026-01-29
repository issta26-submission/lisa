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
//<ID> 761
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"a\": 10, \"b\": true , \"c\":\"x y\"}";
    size_t buf_len = strlen(raw_json) + 1;
    char *minified_buf = (char *)cJSON_malloc(buf_len);
    memcpy(minified_buf, raw_json, buf_len);
    cJSON_Minify(minified_buf);
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(settings, "flag", true_item);
    cJSON *settings_dup = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObjectCS(root, "settings", settings_dup);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *parsed = cJSON_Parse(minified_buf);
    cJSON *parsed_dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObjectCS(root, "parsed", parsed_dup);
    cJSON *parsed_ref = cJSON_CreateObjectReference(parsed);
    cJSON_AddItemToObjectCS(root, "parsed_ref", parsed_ref);

    // step 3: Operate
    cJSON *settings_ref = cJSON_CreateObjectReference(settings);
    cJSON_AddItemToObjectCS(root, "settings_ref", settings_ref);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(minified_buf);
    cJSON_Delete(root);
    return 66;
}