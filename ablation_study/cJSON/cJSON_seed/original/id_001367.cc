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
//<ID> 1367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"nums\":[1,2,3.5],\"meta\":42}";
    size_t json_len = strlen(json);
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);
    float floats[] = {4.0f, 5.5f, 6.25f, 7.75f};
    cJSON *float_arr = cJSON_CreateFloatArray(floats, 4);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *nums_arr = cJSON_GetObjectItem(parsed, "nums");
    cJSON *nums_ref = cJSON_CreateArrayReference(nums_arr);
    cJSON_AddItemToObject(root, "nums_ref", nums_ref);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON *meta_item = cJSON_GetObjectItem(parsed, "meta");
    cJSON_bool meta_is_num = cJSON_IsNumber(meta_item);
    cJSON *meta_is_num_bool = cJSON_CreateBool(meta_is_num);
    cJSON_AddItemToObject(root, "meta_is_number", meta_is_num_bool);

    // step 3: Operate
    int nums_count = cJSON_GetArraySize(nums_arr);
    cJSON *third = cJSON_GetArrayItem(nums_arr, 2);
    double third_val = cJSON_GetNumberValue(third);
    cJSON *combined = cJSON_CreateNumber(third_val + (double)nums_count);
    cJSON_AddItemToObject(root, "combined", combined);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}