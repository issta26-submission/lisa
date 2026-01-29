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
//<ID> 906
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "nothing", null_item);
    cJSON *flag_item = cJSON_AddBoolToObject(root, "flag", 1);
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_Minify(snapshot);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *got_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON *got_enabled = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool flag_is_bool = cJSON_IsBool(got_flag);
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);
    cJSON_AddBoolToObject(parsed, "flag_is_bool", flag_is_bool);
    cJSON_AddBoolToObject(parsed, "enabled_is_bool", enabled_is_bool);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}