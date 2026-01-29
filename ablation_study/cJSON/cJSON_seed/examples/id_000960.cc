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
//<ID> 960
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("deviceX");

    // step 2: Configure
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToObject(root, "info", info);
    cJSON *alive = cJSON_AddFalseToObject(root, "alive");

    // step 3: Operate & Validate
    const char *orig_name = cJSON_GetStringValue(name);
    cJSON_AddItemToObject(root, "original_name", cJSON_CreateString(orig_name));
    char *set_result = cJSON_SetValuestring(name, "deviceY");
    cJSON_AddItemToObject(root, "set_result_copy", cJSON_CreateString(set_result));
    const char *new_name = cJSON_GetStringValue(name);
    cJSON_AddItemToObject(root, "name_copy", cJSON_CreateString(new_name));
    cJSON *alive_item = cJSON_GetObjectItem(root, "alive");
    cJSON_bool alive_was_false = cJSON_IsFalse(alive_item);
    cJSON_AddItemToObject(root, "alive_was_false", cJSON_CreateBool(alive_was_false));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}