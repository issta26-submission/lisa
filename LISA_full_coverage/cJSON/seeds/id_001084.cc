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
//<ID> 1084
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"data\":[1,2,3],\"meta\":{\"name\":\"test\"}}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *extra = cJSON_AddObjectToObject(root, "extra");
    cJSON *active_false = cJSON_AddFalseToObject(extra, "active");
    cJSON *arr = cJSON_GetObjectItem(root, "data");
    cJSON *raw = cJSON_CreateRaw("unparsed_raw_segment");
    cJSON_AddItemToArray(arr, raw);

    // step 3: Operate
    cJSON_bool raw_check = cJSON_IsRaw(raw);
    int count = cJSON_GetArraySize(arr);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(extra, "count", count_num);
    char *out = cJSON_Print(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);
    cJSON_free(out);
    cJSON_free(prealloc);

    // step 4: Validate & Cleanup
    (void)raw_check;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}