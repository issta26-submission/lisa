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
//<ID> 590
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    const char *tags_arr[] = { "alpha", "beta", "gamma" };
    cJSON *tags = cJSON_CreateStringArray(tags_arr, 3);
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "tags", tags);
    cJSON *meta = cJSON_CreateObject();
    cJSON *vernum = cJSON_CreateNumber(1.234);
    cJSON_AddItemToObject(meta, "vernum", vernum);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_name = cJSON_HasObjectItem(parsed, "name");
    cJSON *parsed_name_item = cJSON_GetObjectItem(parsed, "name");
    const char *parsed_name = cJSON_GetStringValue(parsed_name_item);
    cJSON_bool parsed_invalid = cJSON_IsInvalid(parsed);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has_name=%d;invalid=%d;name=%s",
            version ? version : "null",
            (int)has_name,
            (int)parsed_invalid,
            parsed_name ? parsed_name : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}