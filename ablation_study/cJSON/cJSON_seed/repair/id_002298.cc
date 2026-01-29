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
//<ID> 2298
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
    cJSON *fitem = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, fitem);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", "1.0");

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(items));
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 3: Operate
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_AddItemToObject(root, "has_items", has_items ? cJSON_CreateTrue() : cJSON_CreateFalse());
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_false = cJSON_IsFalse(first);
    cJSON_AddItemToObject(root, "first_is_false", first_is_false ? cJSON_CreateTrue() : cJSON_CreateFalse());

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = has_items ? '1' : '0';
    buffer[2] = first_is_false ? '1' : '0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}