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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *num_a = cJSON_CreateNumber(3.1415);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToArray(arr, num_a);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, raw_item);

    // step 2: Configure
    cJSON *num_b = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_b);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    const char *extracted = cJSON_GetStringValue(str_item);
    cJSON_AddStringToObject(meta, "extracted", extracted);
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(meta, "raw_is_raw", raw_flag);
    cJSON_bool invalid_flag = cJSON_IsInvalid(raw_item);
    cJSON_AddBoolToObject(meta, "raw_is_invalid", invalid_flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddBoolToObject(meta, "printed_prealloc", prealloc_ok);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}