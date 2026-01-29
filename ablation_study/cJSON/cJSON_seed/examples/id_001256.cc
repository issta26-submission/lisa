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
//<ID> 1256
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
    cJSON_AddStringToObject(child, "greeting", "hello");
    cJSON_AddBoolToObject(child, "active", 1);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "list", list);
    cJSON *ref = cJSON_CreateStringReference("ref-1");
    cJSON_AddItemToArray(list, ref);

    // step 2: Configure
    cJSON *ref2 = cJSON_CreateStringReference("ref-2");
    cJSON_AddItemToArray(list, ref2);
    cJSON_AddStringToObject(child, "note", "temporary");

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(list, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(child, "greeting");
    cJSON_AddItemToObject(root, "moved_greeting", detached);
    cJSON_AddStringToObject(root, "first_in_list", first_str);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}