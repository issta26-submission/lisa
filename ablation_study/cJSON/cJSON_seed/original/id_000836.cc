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
//<ID> 836
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char src[] = "{\"title\":\"example\",\"active\":false,\"items\":[{\"id\":1},{\"id\":2}],\"flag\":null}";
    cJSON *parsed = cJSON_ParseWithLength(src, sizeof(src) - 1);
    cJSON *items = cJSON_GetObjectItem(parsed, "items");
    cJSON_bool items_is_array = cJSON_IsArray(items);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *items_ref = cJSON_CreateArrayReference(items);
    cJSON_AddItemToObject(root, "copied_items", items_ref);
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(100.0);
    cJSON *n2 = cJSON_CreateNumber(200.0);
    cJSON_AddItemToArray(new_arr, n1);
    cJSON_AddItemToArray(new_arr, n2);
    cJSON_AddItemToObject(root, "additional", new_arr);
    cJSON_AddRawToObject(root, "raw_blob", "{\"raw\":123}");
    cJSON_AddBoolToObject(root, "enabled", items_is_array);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    int original_sz = cJSON_GetArraySize(items);
    int additional_sz = cJSON_GetArraySize(new_arr);
    (void)original_sz;
    (void)additional_sz;
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 4: Validate & Cleanup
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(got_enabled);
    (void)enabled_true;
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}