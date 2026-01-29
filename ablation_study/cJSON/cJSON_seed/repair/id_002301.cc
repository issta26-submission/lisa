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
//<ID> 2301
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
    cJSON_AddStringToObject(root, "name", "device-x");
    cJSON_AddNumberToObject(root, "count", 0.0);

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateString("beta"));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "type", "collection");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_ReplaceItemInObject(root, "count", cJSON_CreateNumber(3.0));

    // step 3: Operate
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON_AddItemToObject(root, "items_is_array", cJSON_CreateBool(items_is_array));
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_is_string = cJSON_IsString(second);
    char *second_val = cJSON_GetStringValue(second);
    cJSON_AddStringToObject(root, "second_value", second_val);
    cJSON *copied_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", copied_items);

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = second_is_string ? '1' : '0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}