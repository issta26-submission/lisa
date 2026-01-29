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
//<ID> 800
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
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, flag);
    const char *json_text = "{\"delete_me\":\"temp\",\"keep\":\"persist\",\"list\":[1,2,3]}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *str_ref = cJSON_CreateStringReference("external_ref");
    cJSON_AddItemToObject(root, "ref", str_ref);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "delete_me");
    cJSON *list_ptr = cJSON_GetObjectItem(parsed, "list");
    cJSON *imported_list = cJSON_Duplicate(list_ptr, 1);
    cJSON_AddItemToObject(root, "imported_list", imported_list);

    // step 3: Operate and Validate
    int items_count = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_false = cJSON_IsFalse(first_item);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}