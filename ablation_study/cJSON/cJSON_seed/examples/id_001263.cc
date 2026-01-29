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
//<ID> 1263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *flag_old = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "config", nested);
    cJSON_AddItemToObject(nested, "enabled", flag_old);

    // step 2: Configure
    cJSON *flag_new = cJSON_CreateBool(1);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(nested, "enabled", flag_new);
    cJSON_bool added_temp = cJSON_AddItemToObject(root, "temporary", cJSON_CreateBool(0));

    // step 3: Operate & Validate
    cJSON *got_enabled = cJSON_GetObjectItem(nested, "enabled");
    cJSON_bool invalid_flag = cJSON_IsInvalid(got_enabled);
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *out = cJSON_PrintUnformatted(dup);
    cJSON_free(out);
    cJSON_Delete(dup);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}