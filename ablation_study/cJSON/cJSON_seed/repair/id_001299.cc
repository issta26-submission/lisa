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
//<ID> 1299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *user = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *active = cJSON_CreateFalse();
    cJSON_AddItemToObject(user, "name", name);
    cJSON_AddItemToObject(root, "user", user);
    cJSON_AddItemToObject(root, "active", active);
    cJSON *status = cJSON_CreateString("ready");
    cJSON_AddItemToObject(root, "status", status);

    // step 2: Configure
    cJSON *user_copy = cJSON_Duplicate(user, 1);
    cJSON_bool are_users_equal = cJSON_Compare(user, user_copy, 1);
    cJSON_bool active_is_false = cJSON_IsFalse(active);
    cJSON_bool active_is_bool = cJSON_IsBool(active);
    char *name_val = (char *)cJSON_GetStringValue(name);

    // step 3: Operate and Validate
    char *updated = cJSON_SetValuestring(status, "running");
    cJSON_bool status_self_compare = cJSON_Compare(status, status, 1);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = name_val ? name_val[0] : 0;
    ((char *)scratch)[2] = updated ? updated[0] : 0;
    ((char *)scratch)[3] = (char)(are_users_equal + active_is_false + active_is_bool + status_self_compare);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(user_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}