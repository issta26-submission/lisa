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
//<ID> 458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *entry = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "entry", entry);
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(entry, "name", name);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(entry, "enabled", enabled);

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("updated_name");
    cJSON *name_ptr = cJSON_GetObjectItem(entry, "name");
    cJSON_ReplaceItemViaPointer(entry, name_ptr, new_name);
    const char *name_value = cJSON_GetStringValue(new_name);
    (void)name_value;

    // step 3: Operate
    cJSON *enabled_item = cJSON_GetObjectItem(entry, "enabled");
    cJSON_bool was_false = cJSON_IsFalse(enabled_item);
    (void)was_false;
    cJSON *note = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(entry, "note", note);

    // step 4: Validate and Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}