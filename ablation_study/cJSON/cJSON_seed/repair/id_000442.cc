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
//<ID> 442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(meta, "creator", "fuzzer");
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateString("beta"));
    cJSON_AddItemToArray(items, cJSON_CreateString("gamma"));
    cJSON_AddStringToObject(meta, "note", "configured");

    // step 3: Operate
    cJSON *meta_from_root = cJSON_GetObjectItem(root, "meta");
    cJSON *creator_item = cJSON_GetObjectItem(meta_from_root, "creator");
    cJSON_bool creator_is_str = cJSON_IsString(creator_item);
    (void)creator_is_str;
    cJSON_DeleteItemFromArray(items, 1);

    // step 4: Validate and Cleanup
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(root, "first_item", first_str);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}