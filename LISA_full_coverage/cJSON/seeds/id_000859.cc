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
//<ID> 859
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *name_ref = cJSON_CreateStringReference("original_name");
    cJSON_AddItemToObject(child, "name", name_ref);
    cJSON_AddTrueToObject(child, "enabled");

    // step 2: Configure
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed_ok;
    cJSON *replacement_name = cJSON_CreateString("replaced_name");
    cJSON_bool replace_ok = cJSON_ReplaceItemInObject(child, "name", replacement_name);
    (void)replace_ok;
    char *current_name = cJSON_GetStringValue(cJSON_GetObjectItem(child, "name"));
    cJSON *new_ref = cJSON_CreateStringReference(current_name);
    cJSON_AddItemToObject(root, "name_ref", new_ref);

    // step 3: Operate
    cJSON_DeleteItemFromObjectCaseSensitive(child, "enabled");
    cJSON_bool is_name_string = cJSON_IsString(cJSON_GetObjectItem(child, "name"));
    (void)is_name_string;
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}