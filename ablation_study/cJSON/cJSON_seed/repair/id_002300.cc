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
//<ID> 2300
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
    cJSON_AddStringToObject(root, "name", "sensor-A");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "type", "collection");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_array = cJSON_IsArray(got_items);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    cJSON_bool name_is_string = cJSON_IsString(got_name);
    char *name_val = cJSON_GetStringValue(got_name);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = items_is_array ? '1' : '0';
    buffer[2] = (name_is_string && name_val && name_val[0]) ? name_val[0] : '\0';
    int arr_size = cJSON_GetArraySize(items);
    buffer[3] = (char)('0' + (arr_size % 10));
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}