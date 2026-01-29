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
//<ID> 1179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"num\": 3.5, \"msg\": \"hello\"} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 0);
    double values_arr[4] = {10.0, 20.5, 30.25, 40.75};

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *double_array = cJSON_CreateDoubleArray(values_arr, 4);
    cJSON_AddItemToObject(root, "double_values", double_array);
    cJSON *raw_item = cJSON_CreateRaw("UNPARSED_RAW_CONTENT");
    cJSON_AddItemToObject(root, "raw_blob", raw_item);

    // step 3: Operate
    cJSON *num_item = cJSON_GetObjectItem(parsed, "num");
    cJSON_bool num_is_number = cJSON_IsNumber(num_item);
    double num_val = cJSON_GetNumberValue(num_item);
    cJSON *doubled = cJSON_CreateNumber(num_val * 2.0);
    cJSON_AddItemToObject(root, "num_doubled", doubled);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw_flag", raw_is_raw);
    cJSON_AddBoolToObject(root, "num_is_number_flag", num_is_number);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}