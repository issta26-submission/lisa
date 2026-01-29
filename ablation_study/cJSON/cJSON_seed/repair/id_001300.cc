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
//<ID> 1300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *profile = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Eve");
    cJSON *id = cJSON_CreateNumber(123.0);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(profile, "name", name);
    cJSON_AddItemToObject(profile, "id", id);
    cJSON_AddStringToObject(profile, "role", "admin");
    cJSON_AddItemToObjectCS(root, "profile", profile);
    cJSON_AddItemToObjectCS(root, "active", active);

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    cJSON *role_item = cJSON_GetObjectItem(profile, "role");
    char *role_val = cJSON_GetStringValue(role_item);

    // step 3: Operate and Validate
    cJSON_bool active_is_bool = cJSON_IsBool(active);
    cJSON_bool active_is_true = cJSON_IsTrue(active);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = role_val ? role_val[0] : 0;
    ((char *)scratch)[2] = (char)(active_is_bool + active_is_true);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}