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
//<ID> 234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToArray(values, num_item);
    cJSON_AddItemToArray(values, str_item);
    cJSON_AddItemToArray(values, raw_item);
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "values", values);
    cJSON *dup_config = cJSON_Duplicate(config, 1);

    // step 3: Operate & Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_values = cJSON_GetObjectItem(got_config, "values");
    int value_count = cJSON_GetArraySize(got_values);
    cJSON *count_num = cJSON_CreateNumber((double)value_count);
    cJSON_AddItemToObject(dup_config, "value_count", count_num);
    cJSON *third_item = cJSON_GetArrayItem(got_values, 2);
    cJSON_bool third_is_raw = cJSON_IsRaw(third_item);
    cJSON *israw_num = cJSON_CreateNumber((double)third_is_raw);
    cJSON_AddItemToObject(root, "third_is_raw", israw_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(dup_config);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}