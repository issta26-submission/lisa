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
//<ID> 454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *item_old = cJSON_CreateString("initial_value");
    cJSON_AddItemToObject(root, "name", item_old);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag_false);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("updated_value");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, item_old, replacement);
    cJSON *note = cJSON_CreateString("this_is_a_note");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate
    const char *name_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    cJSON_bool active_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "active"));
    cJSON *name_copy = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    double status_code = 100.0 + (active_is_false ? 1.0 : 0.0);
    cJSON_AddNumberToObject(root, "status_code", status_code);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced);

    // step 4: Validate and Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}