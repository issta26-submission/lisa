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
//<ID> 1172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\": 1.5, \"b\": [2.5, 3.5]} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 0);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *a_item = cJSON_GetObjectItem(parsed, "a");
    double a_val = cJSON_GetNumberValue(a_item);
    double arr_vals[3];
    arr_vals[0] = a_val;
    arr_vals[1] = 4.5;
    arr_vals[2] = 5.5;
    cJSON *dbl_arr = cJSON_CreateDoubleArray(arr_vals, 3);
    cJSON_AddItemToObject(root, "doubles", dbl_arr);
    cJSON *raw_item = cJSON_CreateRaw("UNPARSED_RAW");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate
    cJSON_bool a_is_num = cJSON_IsNumber(a_item);
    cJSON_AddBoolToObject(root, "a_is_number", a_is_num);
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