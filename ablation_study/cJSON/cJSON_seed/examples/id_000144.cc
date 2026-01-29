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
//<ID> 144
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

    // step 2: Configure
    cJSON *ver = cJSON_CreateString("1.2.3");
    cJSON_AddItemToObject(config, "version", ver);
    cJSON *opt = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "optional", opt);
    cJSON *mode = cJSON_CreateString("release");
    cJSON_AddItemToObject(root, "mode", mode);

    // step 3: Operate & Validate
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON *has_config_item = cJSON_CreateBool(has_config);
    cJSON_AddItemToObject(root, "has_config", has_config_item);
    cJSON *retrieved_config = cJSON_GetObjectItem(root, "config");
    cJSON_bool retrieved_is_object = cJSON_IsObject(retrieved_config);
    cJSON *is_object_item = cJSON_CreateBool(retrieved_is_object);
    cJSON_AddItemToObject(root, "config_is_object", is_object_item);
    cJSON *ver_item = cJSON_GetObjectItem(retrieved_config, "version");
    const char *ver_str = cJSON_GetStringValue(ver_item);
    cJSON *ver_copy = cJSON_CreateString(ver_str);
    cJSON_AddItemToObject(root, "version_copy", ver_copy);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}