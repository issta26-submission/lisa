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
//<ID> 889
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
    cJSON *name_item = cJSON_CreateString("cjson_fuzz_app");
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON_AddNumberToObject(config, "version", 3.14);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "primary_child", child);
    cJSON *child_name = cJSON_CreateString("child_node");
    cJSON_AddItemToObject(child, "id", child_name);

    // step 3: Operate and Validate
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "child_ref", child);
    cJSON_AddNumberToObject(root, "ref_added_flag", (double)ref_added);
    const char *retrieved_name = cJSON_GetStringValue(name_item);
    double name_length = (double)strlen(retrieved_name);
    cJSON_AddNumberToObject(root, "name_length", name_length);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}