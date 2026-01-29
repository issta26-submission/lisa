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
//<ID> 1176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\": 1.25, \"b\": 2.5} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 0);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *item_a = cJSON_GetObjectItem(parsed, "a");
    cJSON *item_b = cJSON_GetObjectItem(parsed, "b");
    double val_a = cJSON_GetNumberValue(item_a);
    double val_b = cJSON_GetNumberValue(item_b);
    double nums[3];
    nums[0] = val_a;
    nums[1] = val_b;
    nums[2] = val_a + val_b;
    cJSON *darray = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "double_array", darray);
    cJSON *raw_item = cJSON_CreateRaw("UNPARSED_RAW");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate
    cJSON_bool a_is_number = cJSON_IsNumber(item_a);
    cJSON_AddBoolToObject(root, "a_is_number", a_is_number);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_is_raw);

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