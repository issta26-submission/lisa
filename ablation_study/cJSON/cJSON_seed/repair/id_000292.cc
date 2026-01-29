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
//<ID> 292
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"status\":\"ok\",\"enabled\":true,\"nested\":{\"name\":\"alpha\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *nested = cJSON_GetObjectItem(root, "nested");
    cJSON *name_item = cJSON_GetObjectItem(nested, "name");
    cJSON *replacement = cJSON_CreateString("beta");
    cJSON_ReplaceItemViaPointer(nested, name_item, replacement);
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    char *status_val = cJSON_GetStringValue(status_item);
    cJSON_AddStringToObject(root, "status_copy", status_val);

    // step 3: Operate and Validate
    int root_is_obj = (int)cJSON_IsObject(root);
    int enabled_flag = (int)cJSON_IsTrue(enabled_item);
    cJSON *name_after = cJSON_GetObjectItem(nested, "name");
    char *name_val = cJSON_GetStringValue(name_after);
    double computed = 2.0 + (double)root_is_obj + (double)enabled_flag;
    cJSON_AddNumberToObject(root, "computed", computed);
    cJSON_AddStringToObject(root, "name_copy", name_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}