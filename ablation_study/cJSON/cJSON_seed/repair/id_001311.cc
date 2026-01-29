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
//<ID> 1311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *enabled_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled_flag);
    cJSON *explicit_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "explicit_null", explicit_null);
    cJSON *convenience_null = cJSON_AddNullToObject(root, "convenience_null");

    // step 2: Configure
    cJSON *settings = cJSON_CreateObject();
    cJSON *feature_x = cJSON_CreateBool(0);
    cJSON_AddItemToObject(settings, "feature_x", feature_x);
    cJSON_AddItemToObject(root, "settings", settings);

    // step 3: Operate and Validate
    cJSON *settings_ptr = cJSON_GetObjectItem(root, "settings");
    cJSON *detached_settings = cJSON_DetachItemViaPointer(root, settings_ptr);
    cJSON_AddItemToObject(root, "settings_copy", detached_settings);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    cJSON_bool has_conv_null = cJSON_HasObjectItem(root, "convenience_null");
    ((char *)scratch)[1] = (char)('0' + (has_conv_null ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}