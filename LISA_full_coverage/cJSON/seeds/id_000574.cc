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
//<ID> 574
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"items\": [ {\"name\":\"alpha\",\"val\":1}, {\"name\":\"beta\",\"val\":2} ], \"meta\": { \"count\": 2 } }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *count = cJSON_GetObjectItem(meta, "count");
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddStringToObject(extra, "note", "added");
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *name_item = cJSON_GetObjectItem(first_item, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    size_t name_len = strlen(name_str) + 1;
    char *local_name = (char *)cJSON_malloc(name_len);
    memset(local_name, 0, name_len);
    strcpy(local_name, name_str);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool are_equal = cJSON_Compare(root, dup, 1);
    cJSON_bool dup_invalid = cJSON_IsInvalid(dup);

    // step 4: Validate & Cleanup
    (void)count;
    (void)are_equal;
    (void)dup_invalid;
    (void)name_str;
    cJSON_free(local_name);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}