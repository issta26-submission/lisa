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
//<ID> 851
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
    cJSON *name_ref = cJSON_CreateStringReference("example_name");
    cJSON_AddItemToObject(child, "name", name_ref);
    cJSON_AddTrueToObject(child, "enabled");
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(child, "count", count);
    cJSON_AddStringToObject(root, "obsolete", "to_be_removed");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateObject();
    cJSON *repl_name_ref = cJSON_CreateStringReference("replaced_name");
    cJSON_AddItemToObject(replacement, "name", repl_name_ref);
    cJSON_AddFalseToObject(replacement, "active");
    cJSON_AddStringToObject(replacement, "source", "auto");

    // step 3: Operate
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "child", replacement);
    (void)replaced;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "obsolete");
    cJSON *current_child = cJSON_GetObjectItem(root, "child");
    const char *final_name = cJSON_GetStringValue(cJSON_GetObjectItem(current_child, "name"));
    (void)final_name;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}