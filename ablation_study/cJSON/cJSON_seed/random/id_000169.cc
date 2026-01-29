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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *config = (cJSON *)0;
    cJSON *version = (cJSON *)0;
    cJSON *retrieved_config = (cJSON *)0;
    cJSON *retrieved_version = (cJSON *)0;
    cJSON *config_is_obj_num = (cJSON *)0;
    cJSON *version_is_obj_num = (cJSON *)0;
    cJSON_bool config_is_object = (cJSON_bool)0;
    cJSON_bool version_is_object = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    config = cJSON_CreateObject();
    version = cJSON_CreateNumber(1.23);

    // step 3: Configure
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "version", version);

    // step 4: Operate
    retrieved_config = cJSON_GetObjectItem(root, "config");
    retrieved_version = cJSON_GetObjectItem(config, "version");
    config_is_object = cJSON_IsObject(retrieved_config);
    version_is_object = cJSON_IsObject(retrieved_version);

    // step 5: Validate
    config_is_obj_num = cJSON_CreateNumber((double)config_is_object);
    version_is_obj_num = cJSON_CreateNumber((double)version_is_object);
    cJSON_AddItemToObject(root, "config_is_object", config_is_obj_num);
    cJSON_AddItemToObject(root, "version_is_object", version_is_obj_num);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}