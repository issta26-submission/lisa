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
//<ID> 1082
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1,2,3],\"info\":\"example\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *values_array = cJSON_GetObjectItem(root, "values");
    int values_count = cJSON_GetArraySize(values_array);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *enabled_item = cJSON_AddFalseToObject(meta, "enabled");
    cJSON *raw_item = cJSON_CreateRaw("<raw>payload</raw>");
    cJSON_AddItemToObject(meta, "raw_data", raw_item);
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    cJSON *count_item = cJSON_CreateNumber((double)values_count);
    cJSON_AddItemToObject(meta, "values_count", count_item);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root_copy, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(root_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}