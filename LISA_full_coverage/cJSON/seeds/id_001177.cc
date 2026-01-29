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
//<ID> 1177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_with_trailing = "{\"a\": 10, \"b\": 20} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_with_trailing, &parse_end, 0);
    const double numbers[] = {3.1415, 2.71828, 1.41421};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "doubles", double_array);
    cJSON *raw_item = cJSON_CreateRaw("RAW_CONTENT");
    cJSON_AddItemToObject(root, "raw_entry", raw_item);

    // step 3: Operate
    cJSON *item_a = cJSON_GetObjectItem(parsed, "a");
    cJSON_bool a_is_num = cJSON_IsNumber(item_a);
    cJSON_AddBoolToObject(root, "a_is_number", a_is_num);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_is_raw);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}