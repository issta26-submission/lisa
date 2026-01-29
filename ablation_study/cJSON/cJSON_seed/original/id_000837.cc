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
//<ID> 837
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[1,2,3],\"flag\":false}";
    size_t json_len = sizeof("{\"items\":[1,2,3],\"flag\":false}") - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *items = cJSON_GetObjectItem(root, "items");

    // step 2: Configure
    cJSON *new_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "new_array", new_arr);
    cJSON_bool new_arr_is_array = cJSON_IsArray(new_arr);
    cJSON_AddBoolToObject(root, "new_array_is_array", new_arr_is_array);
    cJSON_AddRawToObject(root, "raw_fragment", "{\"a\":1,\"b\":true}");

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON_AddBoolToObject(root, "items_is_array", items_is_array);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}