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
//<ID> 833
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "env", "fuzz");
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    const char *json_text = "{\"device\":{\"id\":\"dev123\",\"enabled\":false},\"notes\":\"initial\"}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *device = cJSON_GetObjectItem(parsed, "device");
    cJSON *id_item = cJSON_GetObjectItem(device, "id");
    char *id_value = cJSON_GetStringValue(id_item);
    cJSON_AddStringToObject(root, "device_id", id_value);
    cJSON *detached_notes = cJSON_DetachItemFromObject(parsed, "notes");
    cJSON_AddItemToObject(root, "imported_notes", detached_notes);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}