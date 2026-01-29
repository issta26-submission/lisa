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
//<ID> 1101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"old\",\"value\":42.0,\"raw\":\"raw_text\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    double nums_arr[] = {1.1, 2.2, 3.3};
    cJSON *double_array = cJSON_CreateDoubleArray(nums_arr, 3);
    cJSON_AddItemToObject(root, "doubles", double_array);
    cJSON *true_item = cJSON_AddTrueToObject(root, "enabled");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *setres = cJSON_SetValuestring(name_item, "new_name");

    // step 3: Operate
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    cJSON_bool value_is_num = cJSON_IsNumber(value_item);
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    (void)value_num;
    (void)value_is_num;
    (void)raw_is_raw;
    (void)setres;
    (void)true_item;

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}