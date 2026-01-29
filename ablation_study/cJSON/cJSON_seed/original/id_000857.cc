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
//<ID> 857
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);
    cJSON *initial_name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(container, "name", initial_name);
    cJSON *enabled_flag = cJSON_AddTrueToObject(container, "enabled");

    // step 2: Configure
    cJSON *name_ref = cJSON_CreateStringReference("referenced_name");
    cJSON_ReplaceItemInObject(container, "name", name_ref);
    char buf[256];
    memset(buf, 0, sizeof buf);

    // step 3: Operate
    cJSON_bool ok = cJSON_PrintPreallocated(root, buf, (int)sizeof buf, 1);
    (void)ok;
    const char *current_name = cJSON_GetStringValue(cJSON_GetObjectItem(container, "name"));
    (void)current_name;

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(container, "enabled");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}