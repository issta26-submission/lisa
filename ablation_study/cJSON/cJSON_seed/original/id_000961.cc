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
//<ID> 961
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"arr\": [true, false], \"meta\": {\"raw\": \"placeholder\"}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemReferenceToArray(arr, false_item);
    cJSON *added_raw = cJSON_AddRawToObject(root, "injected_raw", "{\"injected\":true}");

    // step 3: Operate
    cJSON *injected = cJSON_GetObjectItem(root, "injected_raw");
    cJSON_bool raw_flag = cJSON_IsRaw(injected);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON_DeleteItemFromObjectCaseSensitive(meta, "raw");
    cJSON_AddBoolToObject(meta, "injected_is_raw", raw_flag);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_free(unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}