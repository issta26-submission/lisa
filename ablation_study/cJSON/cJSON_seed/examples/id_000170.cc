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
//<ID> 170
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

    // step 2: Configure
    const char *const fruits[] = { "apple", "banana", "cherry" };
    cJSON *fruits_array = cJSON_CreateStringArray(fruits, 3);
    cJSON_AddItemToObject(root, "fruits", fruits_array);
    cJSON *owner = cJSON_CreateString("alice");
    cJSON_AddItemToObject(meta, "owner", owner);
    cJSON *placeholder = cJSON_CreateString("placeholder");
    cJSON_AddItemToObject(root, "owner_copy", placeholder);

    // step 3: Operate & Validate
    cJSON *got_owner = cJSON_GetObjectItem(meta, "owner");
    char *owner_val = cJSON_GetStringValue(got_owner);
    cJSON *owner_dup = cJSON_CreateString(owner_val);
    cJSON_ReplaceItemInObject(root, "owner_copy", owner_dup);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}