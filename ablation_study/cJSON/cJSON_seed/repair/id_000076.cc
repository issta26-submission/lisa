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
//<ID> 76
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
    cJSON_AddNumberToObject(config, "version", 1.0);

    // step 2: Configure
    cJSON *dup_name = cJSON_Duplicate(name, 1);
    cJSON_AddItemToObject(root, "name_copy", dup_name);
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON_AddNumberToObject(root, "has_config_flag", (double)has_config);
    char *updated_ptr = cJSON_SetValuestring(name, "updated");
    cJSON_AddNumberToObject(root, "updated_first_char_code", (double)(unsigned char)updated_ptr[0]);

    // step 3: Operate and Validate
    cJSON *retrieved_dup = cJSON_GetObjectItem(root, "name_copy");
    cJSON_bool dup_is_string = cJSON_IsString(retrieved_dup);
    cJSON_AddNumberToObject(root, "dup_is_string_flag", (double)dup_is_string);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}