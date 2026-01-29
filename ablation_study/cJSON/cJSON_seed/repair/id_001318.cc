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
//<ID> 1318
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
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(settings, "maybe", maybe_null);
    cJSON_AddItemToObject(settings, "flag", flag);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddNullToObject(root, "global_null");

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemViaPointer(settings, maybe_null);
    cJSON_AddItemToObject(root, "detached_maybe", detached);
    cJSON *settings_copy = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", settings_copy);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *settings_obj = cJSON_GetObjectItem(root, "settings");
    cJSON *flag_item = cJSON_GetObjectItem(settings_obj, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag_item);
    double flag_num = cJSON_GetNumberValue(flag_item);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (flag_is_bool ? 1 : 0));
    ((char *)scratch)[2] = (char)('A' + ((int)flag_num & 31));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}