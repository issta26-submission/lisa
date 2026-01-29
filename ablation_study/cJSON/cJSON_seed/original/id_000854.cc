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
//<ID> 854
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *t = cJSON_AddTrueToObject(meta, "enabled");
    cJSON *sref = cJSON_CreateStringReference("original_value");
    cJSON_AddItemToObject(root, "ref", sref);

    // step 2: Configure
    cJSON *newstr = cJSON_CreateString("replaced_value");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "ref", newstr);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    (void)replaced;
    (void)t;

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(meta, "enabled");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}