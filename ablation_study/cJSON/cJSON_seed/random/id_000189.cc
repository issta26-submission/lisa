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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *active_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *retrieved_active = (cJSON *)0;
    cJSON *active_is_string_flag = (cJSON *)0;
    char *json_text = (char *)0;
    cJSON_bool replace_ok = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    active_item = cJSON_AddFalseToObject(root, "active");
    name_item = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "name", name_item);

    // step 3: Configure
    replacement = cJSON_CreateString("replaced_value");

    // step 4: Operate
    replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "active", replacement);

    // step 5: Validate
    retrieved_active = cJSON_GetObjectItem(root, "active");
    active_is_string_flag = cJSON_CreateString(cJSON_IsString(retrieved_active) ? "true" : "false");
    cJSON_AddItemToObject(root, "active_is_string", active_is_string_flag);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}