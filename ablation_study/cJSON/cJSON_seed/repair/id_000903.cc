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
//<ID> 903
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "maybe_null", maybe_null);

    // step 2: Configure
    cJSON *new_child = cJSON_CreateObject();
    cJSON *id_str = cJSON_CreateString("replaced_id");
    cJSON_AddItemToObject(new_child, "id", id_str);
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(config, name_item, new_child);
    char *set_ret = cJSON_SetValuestring(id_str, "updated_id");

    // step 3: Operate and Validate
    cJSON_bool is_null = cJSON_IsNull(maybe_null);
    cJSON_bool is_obj = cJSON_IsObject(new_child);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced_ok);
    cJSON_AddNumberToObject(root, "null_flag", (double)is_null);
    cJSON_AddNumberToObject(root, "obj_flag", (double)is_obj);
    cJSON_AddStringToObject(root, "id_snapshot", set_ret);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}