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
//<ID> 948
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddStringToObject(config, "name", "fuzzer_node");
    cJSON_AddNumberToObject(config, "version", 3.14);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "tester");
    cJSON *entry_template = cJSON_CreateObject();
    cJSON_AddStringToObject(entry_template, "type", "entry");
    cJSON_AddNumberToObject(entry_template, "value", 99.0);

    // step 3: Operate and Validate
    cJSON *entry_dup = cJSON_Duplicate(entry_template, 1);
    cJSON_InsertItemInArray(items, 0, entry_dup);
    cJSON_AddItemToObject(root, "template", entry_template);
    cJSON_bool cfg_is_obj = cJSON_IsObject(config);
    cJSON_AddNumberToObject(root, "config_is_object", (double)cfg_is_obj);
    cJSON_bool templates_equal = cJSON_Compare(entry_template, entry_dup, 1);
    cJSON_AddNumberToObject(root, "template_equals_dup", (double)templates_equal);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}