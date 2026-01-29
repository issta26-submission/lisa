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
//<ID> 1012
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"config\":{\"enabled\":null},\"name\":\"example\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *enabled = cJSON_GetObjectItem(config, "enabled");
    cJSON_bool enabled_is_null = cJSON_IsNull(enabled);
    cJSON *new_obj = cJSON_CreateObject();
    cJSON *label_str = cJSON_CreateString("created_label");
    cJSON_AddItemToObject(new_obj, "label", label_str);
    cJSON_AddBoolToObject(new_obj, "was_null", enabled_is_null);

    // step 3: Operate
    cJSON_AddItemToObject(root, "new_info", new_obj);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "extracted_name", name_val);
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}