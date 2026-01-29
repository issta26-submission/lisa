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
//<ID> 830
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    const char parsed_literal[] = "{\"raw_key\":\"raw_val\",\"num\":10}";
    size_t parsed_len = sizeof(parsed_literal) - 1;
    cJSON *parsed = cJSON_ParseWithLength(parsed_literal, parsed_len);
    cJSON_AddItemToArray(items, parsed);
    cJSON_bool items_are_array = cJSON_IsArray(items);
    cJSON_AddBoolToObject(root, "items_is_array", items_are_array);
    cJSON_AddRawToObject(root, "meta_raw", "{\"inlined\":true}");

    // step 3: Operate
    int count = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *raw_key_item = cJSON_GetObjectItem(first, "raw_key");
    const char *raw_key_val = cJSON_GetStringValue(raw_key_item);
    cJSON *num_item = cJSON_GetObjectItem(first, "num");
    double num_val = cJSON_GetNumberValue(num_item);
    (void)count;
    (void)raw_key_val;
    (void)num_val;

    // step 4: Validate & Cleanup
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(compact);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}