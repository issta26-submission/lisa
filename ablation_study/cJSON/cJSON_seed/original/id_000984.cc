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
//<ID> 984
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"config\": { \"name\": \"initial\" }, \"flag\": true}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    char *new_value = cJSON_SetValuestring(name_item, "updated");
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_bool settings_is_obj = cJSON_IsObject(settings);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(settings, "enabled", false_item);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, print_buf, 256, 1);
    cJSON_bool false_check = cJSON_IsFalse(false_item);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(settings, "enabled");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "settings");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}