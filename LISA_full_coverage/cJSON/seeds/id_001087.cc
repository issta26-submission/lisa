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
//<ID> 1087
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1,2,3],\"info\":\"demo\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *initial_false = cJSON_AddFalseToObject(meta, "initial_flag");
    cJSON *raw_item = cJSON_CreateRaw("unparsed:raw_payload");
    cJSON_AddItemToObject(root, "raw_blob", raw_item);

    // step 3: Operate
    cJSON *arr = cJSON_GetObjectItem(root, "values");
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *count_node = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(meta, "values_count", count_node);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON *raw_flag_node = cJSON_CreateBool(raw_is_raw);
    cJSON_AddItemToObject(meta, "raw_is_raw", raw_flag_node);

    // step 4: Validate & Cleanup
    char *pretty = cJSON_Print(root);
    int prealloc_len = 256;
    char *prealloc = (char *)cJSON_malloc(prealloc_len);
    memset(prealloc, 0, prealloc_len);
    cJSON_PrintPreallocated(root, prealloc, prealloc_len, 1);
    cJSON_free(prealloc);
    cJSON_free(pretty);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}