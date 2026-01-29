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
//<ID> 1436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"meta\":{\"raw\":\"<data>\",\"val\":42},\"arr\":[1,2,3]}";
    size_t json_len = strlen(json_text);
    cJSON *root = cJSON_ParseWithLength(json_text, json_len);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *raw_item = cJSON_GetObjectItem(meta, "raw");
    const char *raw_str = cJSON_GetStringValue(raw_item);

    // step 2: Configure
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddRawToObject(root, "injected_raw", "{\"injected\":true}");
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *detached_arr = cJSON_DetachItemViaPointer(root, arr);
    cJSON *arr_dup = cJSON_Duplicate(detached_arr, 1);
    cJSON_AddItemToObject(root, "arr_copy", arr_dup);

    // step 3: Operate & Validate
    char *buffer = (char *)cJSON_malloc(1024);
    memset(buffer, 0, 1024);
    cJSON_PrintPreallocated(root, buffer, 1024, 1);
    size_t printed_len = strlen(buffer);
    cJSON_bool meta_equal = cJSON_Compare(meta, meta_dup, 1);
    (void)printed_len;
    (void)meta_equal;
    (void)raw_str;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(detached_arr);
    cJSON_Delete(meta_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}