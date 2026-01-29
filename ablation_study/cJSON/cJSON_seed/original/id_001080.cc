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
//<ID> 1080
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"numbers\":[10,20,30],\"meta\":\"original\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *enabled_false = cJSON_AddFalseToObject(config, "enabled");
    cJSON *raw_blob = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(config, "blob", raw_blob);

    // step 3: Operate
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    int count = cJSON_GetArraySize(numbers);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_blob);
    cJSON *summary = cJSON_AddObjectToObject(root, "summary");
    cJSON *count_item = cJSON_CreateNumber((double)count + (double)raw_is_raw);
    cJSON_AddItemToObject(summary, "count_plus_raw", count_item);

    // step 4: Validate & Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}