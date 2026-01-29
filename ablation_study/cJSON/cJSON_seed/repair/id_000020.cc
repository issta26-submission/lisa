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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(flags, false_item);
    cJSON *old_mode = cJSON_CreateString("temporary");
    cJSON_AddItemToObject(settings, "mode", old_mode);
    cJSON *replacement_false = cJSON_CreateFalse();
    cJSON_ReplaceItemViaPointer(settings, old_mode, replacement_false);

    // step 3: Operate and Validate
    cJSON_bool rep_is_bool = cJSON_IsBool(replacement_false);
    cJSON *first_flag = cJSON_GetArrayItem(flags, 0);
    cJSON_bool flag_is_bool = cJSON_IsBool(first_flag);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    (void)rep_is_bool;
    (void)flag_is_bool;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}