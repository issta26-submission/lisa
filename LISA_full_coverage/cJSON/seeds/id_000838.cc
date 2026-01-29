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
//<ID> 838
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"flags\":true,\"items\":[1,2,3]}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON_AddBoolToObject(root, "items_is_array", items_is_array);
    cJSON *empty_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "empty_array", empty_array);
    int nums[3] = {10, 20, 30};
    cJSON *ints = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON_AddRawToObject(root, "raw_payload", "{\"nested\":123,\"active\":false}");

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    const char raw_to_parse[] = "[false,true]";
    size_t raw_len = sizeof(raw_to_parse) - 1;
    cJSON *parsed_raw = cJSON_ParseWithLength(raw_to_parse, raw_len);
    cJSON_bool parsed_is_array = cJSON_IsArray(parsed_raw);
    cJSON_AddBoolToObject(root, "parsed_raw_is_array", parsed_is_array);
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed_raw);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}