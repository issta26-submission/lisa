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
//<ID> 984
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
    cJSON *name = cJSON_CreateString("device-A");
    cJSON *id = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(meta, "id", id);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *active_flag = cJSON_AddFalseToObject(root, "active");

    // step 3: Operate & Validate
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool was_false = cJSON_IsFalse(got_active);
    cJSON *previous_state = cJSON_CreateBool(was_false);
    cJSON_AddItemToObject(root, "was_active_false", previous_state);
    cJSON *true_flag = cJSON_CreateTrue();
    cJSON_ReplaceItemInObject(root, "active", true_flag);
    cJSON *got_name = cJSON_GetObjectItem(meta, "name");
    char *current_name = cJSON_GetStringValue(got_name);
    cJSON *name_copy = cJSON_CreateString(current_name);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    (void)active_flag; // ensure initial flag reference is used in the flow

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}