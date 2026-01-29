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
//<ID> 1365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\":12.5,\"b\":[7,8,9]}";
    size_t json_len = strlen(json);
    cJSON *parsed = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    float numbers[4] = {1.1f, 2.2f, 3.3f, 4.4f};
    cJSON *float_arr = cJSON_CreateFloatArray(numbers, 4);
    cJSON_AddItemToObject(root, "floats", float_arr);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *b_item = cJSON_GetObjectItem(parsed, "b");
    cJSON *b_ref = cJSON_CreateArrayReference(b_item);
    cJSON_AddItemToObject(root, "b_ref", b_ref);

    // step 3: Operate
    cJSON *a_item = cJSON_GetObjectItem(parsed, "a");
    cJSON_bool a_is_num = cJSON_IsNumber(a_item);
    cJSON *a_is_num_flag = cJSON_CreateNumber((double)a_is_num);
    cJSON_AddItemToObject(root, "a_is_number", a_is_num_flag);
    int floats_count = cJSON_GetArraySize(float_arr);
    cJSON *count_item = cJSON_CreateNumber((double)floats_count);
    cJSON_AddItemToObject(root, "floats_count", count_item);

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