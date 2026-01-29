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
//<ID> 1086
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[1,2,3],\"name\":\"example\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *enabled_flag = cJSON_AddFalseToObject(meta, "enabled");
    cJSON *raw_item_added = cJSON_AddRawToObject(root, "rawval", "{\"raw\":true}");

    // step 3: Operate
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int size = cJSON_GetArraySize(items);
    cJSON *raw_item = cJSON_GetObjectItem(root, "rawval");
    double is_raw_flag = (double)cJSON_IsRaw(raw_item);
    cJSON *computed = cJSON_CreateNumber(is_raw_flag * (double)size);
    cJSON_AddItemToObject(meta, "raw_count", computed);
    char *out = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(prealloc);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}