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
//<ID> 819
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *tags_arr[] = { "alpha", "beta", "gamma" };
    cJSON *tags = cJSON_CreateStringArray(tags_arr, 3);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *bool_item = cJSON_CreateBool(0);
    cJSON_AddNullToObject(root, "missing");
    cJSON_AddItemToObject(root, "tags", tags);
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON_AddItemToObject(root, "visible", bool_item);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *second_tag = cJSON_GetArrayItem(tags, 1);
    const char *extracted = cJSON_GetStringValue(second_tag);
    cJSON *favorite = cJSON_CreateString(extracted);
    cJSON_AddItemToObject(meta, "favorite", favorite);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_fav = cJSON_GetObjectItem(parsed_meta, "favorite");
    const char *parsed_fav_str = cJSON_GetStringValue(parsed_fav);
    (void)parsed_fav_str;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}