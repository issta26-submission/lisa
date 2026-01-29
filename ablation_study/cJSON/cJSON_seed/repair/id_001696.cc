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
//<ID> 1696
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *name_item = cJSON_CreateString("fuzz-target");
    cJSON_AddItemToObject(root, "name", name_item);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(cfg, "items", items);
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON *item_b = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, item_a);
    cJSON_AddItemToArray(items, item_b);
    cJSON *meta = cJSON_AddObjectToObject(cfg, "meta");
    cJSON_AddStringToObject(meta, "note", "configured");

    // step 3: Operate and Validate
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "config");
    cJSON *retr_name = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    int size_before = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(items, 0);
    int size_after = cJSON_GetArraySize(items);
    cJSON *first_after = cJSON_GetArrayItem(items, 0);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)has_cfg);
    buffer[3] = (char)('0' + (size_before - size_after));
    (void)first_after;
    (void)size_before;
    (void)size_after;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}