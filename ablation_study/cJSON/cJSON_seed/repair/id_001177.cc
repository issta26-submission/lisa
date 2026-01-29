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
//<ID> 1177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("device");
    cJSON *enabled_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(settings, "name", name_item);
    cJSON_AddItemToObject(settings, "enabled", enabled_item);
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON *ver_item = cJSON_AddNumberToObject(settings, "version", 1.0);
    cJSON *active_item = cJSON_AddBoolToObject(settings, "active", 1);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *name_ref = cJSON_GetObjectItem(settings, "name");
    char *setret = cJSON_SetValuestring(name_ref, "device-x");
    (void)setret;
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");
    cJSON_bool was_null = cJSON_IsNull(maybe);
    cJSON_AddNumberToObject(root, "computed_flag", (double)was_null + 2.5);
    cJSON_AddBoolToObject(root, "final_active", 0);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}