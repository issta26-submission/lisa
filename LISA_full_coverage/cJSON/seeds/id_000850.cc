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
//<ID> 850
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "active");
    cJSON *name = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *ref = cJSON_CreateStringReference("zero_copy");
    cJSON_AddItemToObject(meta, "ref", ref);

    // step 2: Configure
    cJSON *new_name = cJSON_CreateStringReference("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "name", new_name);
    (void)replaced;

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "active");
    const char *snap = cJSON_GetStringValue(cJSON_GetObjectItem(root, "snapshot"));
    (void)snap;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}