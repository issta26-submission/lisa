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
//<ID> 965
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *mutableStr = cJSON_CreateString("initial");
    cJSON *meta = cJSON_CreateString("meta-data");

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(mutableStr, "device-updated");
    (void)set_ret;
    const char *current_value = cJSON_GetStringValue(mutableStr);
    cJSON *copied_name = cJSON_CreateString(current_value);
    cJSON_AddItemToObject(info, "name", mutableStr);
    cJSON_AddItemToObject(info, "name_copy", copied_name);
    cJSON_AddFalseToObject(info, "enabled");
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *got_info = cJSON_GetObjectItem(root, "info");
    cJSON *got_name = cJSON_GetObjectItem(got_info, "name");
    const char *validated = cJSON_GetStringValue(got_name);
    cJSON_AddItemToObject(root, "validated_name", cJSON_CreateString(validated));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}