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
//<ID> 1301
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
    cJSON_AddItemToObject(root, "user", user);
    cJSON *name_item = cJSON_AddStringToObject(user, "name", "Dana");
    cJSON *active_item = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "active", active_item);
    cJSON *status_item = cJSON_AddStringToObject(root, "status", "initial");

    // step 2: Configure
    cJSON *role_item = cJSON_CreateString("tester");
    cJSON_AddItemToObject(root, "role", role_item);
    cJSON *copied_user = cJSON_Duplicate(user, 1);

    // step 3: Operate and Validate
    cJSON *active_lookup = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_bool = cJSON_IsBool(active_lookup);
    cJSON_bool active_is_true = cJSON_IsTrue(active_lookup);
    char *name_val = cJSON_GetStringValue(name_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = name_val ? name_val[0] : 0;
    ((char *)scratch)[2] = (char)(active_is_bool + active_is_true);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(copied_user);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}