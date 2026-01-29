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
//<ID> 2297
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
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3.0));
    cJSON *enabled_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled_false);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "device-X");
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddNumberToObject(config, "threshold", 42.5);

    // step 3: Operate
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_bool has_missing = cJSON_HasObjectItem(root, "missing");
    cJSON_AddItemToObject(root, "has_items", cJSON_CreateBool(has_items));
    cJSON_AddItemToObject(root, "has_missing", cJSON_CreateBool(has_missing));
    int items_count = cJSON_GetArraySize(items);
    cJSON_AddItemToObject(root, "items_count", cJSON_CreateNumber((double)items_count));
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (items_count >= 10) ? '9' : (char)('0' + (items_count % 10));
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}