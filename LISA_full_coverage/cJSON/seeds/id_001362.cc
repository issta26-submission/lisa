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
//<ID> 1362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"nums\":[4,5,6],\"meta\":{\"value\":42}}";
    size_t json_len = strlen(json);
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    cJSON *nums_item = cJSON_GetObjectItem(parsed, "nums");
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *val_item = cJSON_GetObjectItem(meta, "value");
    float floats[3] = {1.1f, 2.2f, 3.3f};
    cJSON *float_arr = cJSON_CreateFloatArray(floats, 3);
    cJSON *nums_array_ref = cJSON_CreateArrayReference(nums_item);
    cJSON_bool value_is_number = cJSON_IsNumber(val_item);
    int nums_count = cJSON_GetArraySize(nums_item);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "float_array", float_arr);
    cJSON_AddNumberToObject(root, "is_value_number", (double)value_is_number);
    cJSON_AddNumberToObject(root, "nums_count", (double)nums_count);

    // step 3: Operate
    char *printed_root = cJSON_PrintUnformatted(root);
    char *printed_parsed = cJSON_PrintUnformatted(parsed);
    size_t printed_root_len = strlen(printed_root);
    size_t printed_parsed_len = strlen(printed_parsed);
    char *buf_root = (char *)cJSON_malloc(printed_root_len + 1);
    char *buf_parsed = (char *)cJSON_malloc(printed_parsed_len + 1);
    memset(buf_root, 0, printed_root_len + 1);
    memset(buf_parsed, 0, printed_parsed_len + 1);
    memcpy(buf_root, printed_root, printed_root_len + 1);
    memcpy(buf_parsed, printed_parsed, printed_parsed_len + 1);
    cJSON_Minify(buf_root);
    cJSON_Minify(buf_parsed);
    cJSON_free(printed_root);
    cJSON_free(printed_parsed);
    cJSON_free(buf_root);
    cJSON_free(buf_parsed);

    // Add the array reference to parsed after printing to avoid creating a cycle before printing
    cJSON_AddItemToObject(parsed, "nums_ref", nums_array_ref);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    return 66;
}