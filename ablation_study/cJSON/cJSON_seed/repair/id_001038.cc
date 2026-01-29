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
//<ID> 1038
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
    cJSON *note = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "note", note);

    // step 2: Configure
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", flag_true);
    cJSON_AddItemToObject(config, "legacy_disabled", flag_false);
    char *new_note = cJSON_SetValuestring(note, "updated");

    // step 3: Operate and Validate
    cJSON_bool has_enabled = cJSON_HasObjectItem(config, "enabled");
    cJSON_AddBoolToObject(root, "has_enabled", has_enabled);
    cJSON *legacy_item = cJSON_GetObjectItem(config, "legacy_disabled");
    cJSON_bool legacy_is_false = cJSON_IsFalse(legacy_item);
    cJSON_bool legacy_is_bool = cJSON_IsBool(legacy_item);
    cJSON_AddNumberToObject(root, "legacy_disabled_is_false", (double)legacy_is_false);
    cJSON_AddNumberToObject(root, "legacy_disabled_is_bool", (double)legacy_is_bool);
    cJSON_AddNumberToObject(root, "note_first_char", (double)(unsigned char)(new_note ? new_note[0] : '\0'));
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}