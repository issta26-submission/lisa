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
//<ID> 72
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
    cJSON *name = cJSON_CreateString("original");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *version = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(config, "version", version);

    // step 2: Configure
    char *updated_name_ptr = cJSON_SetValuestring(name, "updated");
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON_AddBoolToObject(root, "has_config", has_config);
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", dup_config);

    // step 3: Operate and Validate
    cJSON *retr_config = cJSON_GetObjectItem(root, "config");
    cJSON *retr_name = cJSON_GetObjectItem(retr_config, "name");
    cJSON_AddNumberToObject(root, "first_char_code", (double)(unsigned char)updated_name_ptr[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}