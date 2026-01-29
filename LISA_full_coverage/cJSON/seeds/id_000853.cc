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
//<ID> 853
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON *nameRef = cJSON_CreateStringReference("initial_name");
    cJSON_AddItemToObject(sub, "name", nameRef);
    cJSON_AddTrueToObject(sub, "flag");
    cJSON_AddItemToObject(sub, "count", cJSON_CreateNumber(7.0));

    // step 2: Configure
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *newNameRef = cJSON_CreateStringReference("replaced_name");
    cJSON_ReplaceItemInObject(sub, "name", newNameRef);
    cJSON_AddBoolToObject(root, "print_ok", printed_ok);

    // step 3: Operate
    const char *current_name = cJSON_GetStringValue(cJSON_GetObjectItem(sub, "name"));
    cJSON_AddStringToObject(root, "name_copy", current_name);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(sub, "flag");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}