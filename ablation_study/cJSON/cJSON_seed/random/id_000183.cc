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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *initial_name = (cJSON *)0;
    cJSON *updated_name = (cJSON *)0;
    cJSON *active_flag = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *name_snapshot = (cJSON *)0;
    cJSON_bool replace_ok = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    initial_name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(child, "name", initial_name);

    // step 3: Configure
    active_flag = cJSON_AddFalseToObject(root, "active");
    updated_name = cJSON_CreateString("updated");
    replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(child, "name", updated_name);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(child, "name");
    name_snapshot = cJSON_CreateString(cJSON_GetStringValue(retrieved_name));
    cJSON_AddItemToObject(root, "name_snapshot", name_snapshot);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}