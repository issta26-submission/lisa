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
//<ID> 1175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"val\": 1.5, \"msg\":\"hello\"} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 0);
    double doubles_src[3];
    doubles_src[0] = 3.1415;
    doubles_src[1] = 2.7182;
    doubles_src[2] = -0.0;

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *darr = cJSON_CreateDoubleArray(doubles_src, 3);
    cJSON_AddItemToObject(root, "double_array", darr);
    cJSON *raw_item = cJSON_CreateRaw("UNPARSED_RAW_CONTENT");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate & Validate
    cJSON *val_item = cJSON_GetObjectItem(parsed, "val");
    double val_num = cJSON_GetNumberValue(val_item);
    cJSON *dbl_times_two = cJSON_CreateNumber(val_num * 2.0);
    cJSON_AddItemToObject(root, "val_times_two", dbl_times_two);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_is_raw);
    cJSON_bool val_is_number = cJSON_IsNumber(val_item);
    cJSON_AddBoolToObject(root, "val_is_number", val_is_number);
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Cleanup
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}