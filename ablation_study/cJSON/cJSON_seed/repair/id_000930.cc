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
//<ID> 930
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
    cJSON *raw_meta = cJSON_CreateRaw("{\"flag\":true}");
    cJSON_AddItemToObject(config, "metadata", raw_meta);
    cJSON *attempts = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(config, "attempts", attempts);

    // step 3: Operate and Validate
    cJSON *found_raw = cJSON_GetObjectItemCaseSensitive(config, "metadata");
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddStringToObject(replacement, "note", "converted");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(config, found_raw, replacement);
    (void)replaced;
    cJSON *new_meta = cJSON_GetObjectItemCaseSensitive(config, "metadata");
    cJSON *note_item = cJSON_GetObjectItemCaseSensitive(new_meta, "note");
    char *note_str = cJSON_GetStringValue(note_item);
    cJSON_AddStringToObject(root, "note_snapshot", note_str);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}