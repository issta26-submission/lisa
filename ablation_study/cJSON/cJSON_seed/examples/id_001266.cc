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
//<ID> 1266
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
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateBool(0);

    // step 2: Configure
    cJSON_AddItemToObject(settings, "initial_flag", flag_true);
    cJSON_AddItemToObject(settings, "temp_flag", flag_false);
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *replacement_flag = cJSON_CreateBool(0);
    cJSON_ReplaceItemInObject(settings, "initial_flag", replacement_flag);

    // step 3: Operate & Validate
    cJSON *current_flag = cJSON_GetObjectItem(settings, "initial_flag");
    cJSON_bool was_invalid = cJSON_IsInvalid(current_flag);
    cJSON_AddItemToObject(root, "initial_was_invalid", cJSON_CreateBool(was_invalid));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}