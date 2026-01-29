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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *version = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "version", version);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name, "updated");
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "backup", dup_config);

    // step 3: Operate and Validate
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(config, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON_AddNumberToObject(root, "config_present", (double)has_config);
    cJSON_AddNumberToObject(root, "first_char_code", (double)(unsigned char)name_str[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}