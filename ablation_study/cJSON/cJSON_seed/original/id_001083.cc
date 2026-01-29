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
//<ID> 1083
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[10,20,30],\"raw\":\"<raw>data</raw>\",\"obj\":{}}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *enabled_item = cJSON_AddFalseToObject(settings, "enabled");
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    double info_value = (double)arr_size + ((double)raw_flag * 0.5);
    cJSON *info_num = cJSON_CreateNumber(info_value);
    cJSON_AddItemToObject(settings, "info", info_num);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    char *pretty_copy = cJSON_Print(root_copy);

    // step 4: Validate & Cleanup
    cJSON_bool identical = cJSON_Compare(root, root_copy, 1);
    (void)identical;
    cJSON_free(pretty);
    cJSON_free(pretty_copy);
    cJSON_free(prealloc);
    cJSON_Delete(root_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}