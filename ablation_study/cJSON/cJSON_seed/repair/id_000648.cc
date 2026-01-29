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
//<ID> 648
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metadata", metadata);
    cJSON_AddStringToObject(metadata, "name", "example_name");
    cJSON_AddStringToObject(root, "title", "Example Title");
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    (void)printed_ok;
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 3: Operate and Validate
    cJSON *meta_obj = cJSON_GetObjectItem(root, "metadata");
    cJSON *name_item = cJSON_GetObjectItem(meta_obj, "name");
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "name_copy", name_val);
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool was_false = cJSON_IsFalse(enabled_item);
    cJSON_AddNumberToObject(root, "enabled_was_false", (double)was_false);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}