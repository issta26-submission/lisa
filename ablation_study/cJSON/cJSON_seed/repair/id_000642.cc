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
//<ID> 642
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
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddStringToObject(data, "name", "example_name");
    cJSON *note = cJSON_CreateString("note value");
    cJSON_AddItemToObject(data, "note", note);

    // step 3: Operate and Validate
    cJSON *retrieved_data = cJSON_GetObjectItem(root, "data");
    cJSON *name_item = cJSON_GetObjectItem(retrieved_data, "name");
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "name_copy", name_val);
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_is_false = cJSON_IsFalse(enabled_item);
    cJSON_AddNumberToObject(root, "enabled_false_flag", (double)enabled_is_false);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}