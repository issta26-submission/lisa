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
//<ID> 466
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
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(child, "name", name);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "value", num);

    // step 3: Operate & Validate
    cJSON *retrieved_child = cJSON_GetObjectItem(root, "child");
    cJSON *retrieved_name = cJSON_GetObjectItem(retrieved_child, "name");
    const char *orig_name = cJSON_GetStringValue(retrieved_name);
    char status[128];
    memset(status, 0, sizeof(status));
    sprintf(status, "orig=%s", orig_name ? orig_name : "null");

    cJSON *replacement = cJSON_CreateObject();
    cJSON *new_name = cJSON_CreateString("replaced_name");
    cJSON_AddItemToObject(replacement, "name", new_name);
    cJSON_bool ok = cJSON_ReplaceItemViaPointer(root, child, replacement);
    (void)ok;

    cJSON *new_child = cJSON_GetObjectItem(root, "child");
    cJSON *new_child_name = cJSON_GetObjectItem(new_child, "name");
    const char *replaced_name = cJSON_GetStringValue(new_child_name);
    memset(status, 0, sizeof(status));
    sprintf(status, "now=%s", replaced_name ? replaced_name : "null");

    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}