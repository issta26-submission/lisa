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
//<ID> 856
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

    // step 2: Configure
    cJSON *name_ref = cJSON_CreateStringReference("initial_name");
    cJSON_AddItemToObject(child, "name", name_ref);
    cJSON *active_item = cJSON_AddTrueToObject(child, "active");

    // step 3: Operate
    cJSON *replacement_ref = cJSON_CreateStringReference("updated_name");
    cJSON_ReplaceItemInObject(child, "name", replacement_ref);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(child, "name");
    const char *name_value = cJSON_GetStringValue(name_item);
    cJSON_bool is_active = cJSON_IsTrue(active_item);
    (void)name_value;
    (void)is_active;

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(child, "active");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}