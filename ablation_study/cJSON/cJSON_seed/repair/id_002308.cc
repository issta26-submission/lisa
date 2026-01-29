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
//<ID> 2308
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
    cJSON_AddItemToArray(items, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("gamma"));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "type", "collection");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(items));
    cJSON_AddItemToObject(root, "flag", cJSON_CreateFalse());

    // step 3: Operate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_array = cJSON_IsArray(got_items);
    cJSON_AddItemToObject(root, "items_is_array", cJSON_CreateBool(is_array));
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_item);
    char *first_value = cJSON_GetStringValue(first_item);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (first_is_string ? '1' : '0');
    buffer[2] = (first_value && first_value[0]) ? first_value[0] : '\0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}