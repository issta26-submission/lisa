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
//<ID> 2307
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
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(100.0));
    cJSON *device = cJSON_CreateObject();
    cJSON_AddStringToObject(device, "name", "device-007");
    cJSON_AddItemToArray(items, device);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddNumberToObject(root, "total", 3.0);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateObject());
    cJSON_AddStringToObject(cJSON_GetObjectItem(root, "meta"), "type", "collection");

    // step 3: Operate
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON_AddItemToObject(root, "items_is_array", cJSON_CreateBool(items_is_array));
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_str = cJSON_IsString(first_item);
    char *first_val = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(root, "first_value_copy", first_val);

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = first_is_str ? '1' : '0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}