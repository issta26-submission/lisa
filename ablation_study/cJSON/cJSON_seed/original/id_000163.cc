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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON *b_true = cJSON_CreateTrue();
    cJSON *b_false = cJSON_CreateFalse();
    cJSON_InsertItemInArray(flags, 0, b_true);
    cJSON_InsertItemInArray(flags, 1, b_false);
    cJSON_AddItemToObject(root, "flags", flags);

    // step 2: Configure
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddTrueToObject(config, "enabled");
    cJSON *moved_flag = cJSON_DetachItemFromArray(flags, 0);
    cJSON_AddItemToObject(config, "moved_flag", moved_flag);

    // step 3: Operate and Validate
    int flags_size = cJSON_GetArraySize(flags);
    cJSON *first_flag = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first_flag);
    cJSON_bool moved_is_bool = cJSON_IsBool(moved_flag);
    char *printed = cJSON_PrintUnformatted(root);
    int summary = flags_size + (int)first_is_bool + (int)moved_is_bool + root->type + config->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}