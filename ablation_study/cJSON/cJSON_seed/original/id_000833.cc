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
//<ID> 833
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"example\"}";
    cJSON *root = cJSON_ParseWithLength(initial_json, sizeof(initial_json) - 1);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_bool is_array = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "items_is_array", is_array);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_payload", "{\"nested\": [true, false], \"count\": 2}");

    // step 3: Operate
    char *serialized = cJSON_PrintUnformatted(root);
    cJSON_free(serialized);
    (void)raw_item;
    (void)is_array;

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}