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
//<ID> 1173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_with_trailing = "{\"value\": 7.25, \"label\": \"example\"} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_with_trailing, &parse_end, 0);
    double nums_arr[] = {1.1, 2.2, 3.3};
    cJSON *double_array = cJSON_CreateDoubleArray(nums_arr, 3);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "doubles", double_array);
    cJSON *raw_item = cJSON_CreateRaw("RAW_PAYLOAD");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate
    cJSON *num_item = cJSON_GetObjectItem(parsed, "value");
    double extracted = cJSON_GetNumberValue(num_item);
    cJSON_bool num_check = cJSON_IsNumber(num_item);
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    cJSON *scaled = cJSON_CreateNumber(extracted * 2.0);
    cJSON_AddItemToObject(root, "scaled_value", scaled);
    cJSON_AddBoolToObject(root, "value_is_number", num_check);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_check);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}