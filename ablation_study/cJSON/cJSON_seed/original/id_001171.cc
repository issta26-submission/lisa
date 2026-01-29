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
//<ID> 1171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"pnum\": 7, \"message\":\"ok\"} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 0);
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *double_arr = cJSON_CreateDoubleArray(nums, 3);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "doubles", double_arr);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToObject(root, "raw_blob", raw_item);

    // step 3: Operate
    cJSON *pnum_item = cJSON_GetObjectItem(parsed, "pnum");
    cJSON_bool is_num = cJSON_IsNumber(pnum_item);
    cJSON_AddBoolToObject(root, "pnum_is_number", is_num);
    double pnum_value = cJSON_GetNumberValue(pnum_item);
    cJSON *double_num = cJSON_CreateNumber(pnum_value * 2.0);
    cJSON_AddItemToObject(root, "pnum_times_two", double_num);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_is_raw);

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