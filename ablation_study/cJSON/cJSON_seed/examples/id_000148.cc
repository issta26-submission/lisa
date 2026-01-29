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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("test-name");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "optional", optional_null);
    cJSON *value = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "value", value);

    // step 3: Operate & Validate
    cJSON_bool has_name = cJSON_HasObjectItem(meta, "name");
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(retrieved_meta);
    cJSON_AddBoolToObject(root, "has_name", has_name);
    cJSON_AddBoolToObject(root, "meta_is_object", meta_is_obj);
    cJSON *retrieved_name = cJSON_GetObjectItem(meta, "name");
    char *name_str = cJSON_GetStringValue(retrieved_name);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}