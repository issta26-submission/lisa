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
//<ID> 594
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
    cJSON *owner = cJSON_CreateString("tester");
    cJSON_AddItemToObject(root, "owner", owner);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    int nums_arr[3] = {1, 2, 3};
    cJSON *nums = cJSON_CreateIntArray(nums_arr, 3);
    cJSON_AddItemToObject(root, "nums", nums);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version_major", 1.0);
    cJSON_AddTrueToObject(root, "active");
    cJSON *raw = cJSON_CreateRaw("{\"k\":1}");
    cJSON_AddItemToObject(meta, "raw", raw);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_owner_root = cJSON_HasObjectItem(root, "owner");
    cJSON *parsed_owner = cJSON_GetObjectItem(parsed, "owner");
    const char *owner_str = cJSON_GetStringValue(parsed_owner);
    cJSON_bool owner_is_invalid = cJSON_IsInvalid(parsed_owner);
    cJSON_bool has_owner_parsed = cJSON_HasObjectItem(parsed, "owner");

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has_root=%d;has_parsed=%d;owner=%s;invalid=%d",
            version ? version : "null",
            (int)has_owner_root,
            (int)has_owner_parsed,
            owner_str ? owner_str : "null",
            (int)owner_is_invalid);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}