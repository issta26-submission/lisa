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
//<ID> 239
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
    cJSON *rawdata = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *s1 = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, s1);
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToObject(root, "rawdata", rawdata);
    cJSON_AddStringToObject(config, "mode", "active");

    // step 3: Operate & Validate
    cJSON *values_ptr = cJSON_GetObjectItem(root, "values");
    int values_count = cJSON_GetArraySize(values_ptr);
    cJSON *count_num = cJSON_CreateNumber((double)values_count);
    cJSON_AddItemToObject(root, "values_count", count_num);

    cJSON *raw_ptr = cJSON_GetObjectItem(root, "rawdata");
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_ptr);
    cJSON *raw_flag_num = cJSON_CreateNumber((double)raw_is_raw);
    cJSON_AddItemToObject(root, "raw_is_raw", raw_flag_num);

    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON *dup_mode = cJSON_GetObjectItem(dup_config, "mode");
    const char *dup_mode_str = cJSON_GetStringValue(dup_mode);
    cJSON *dup_mode_copy = cJSON_CreateString(dup_mode_str);
    cJSON_AddItemToObject(root, "dup_mode", dup_mode_copy);

    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(dup_config);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}