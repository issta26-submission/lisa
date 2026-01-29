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
//<ID> 925
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"flag\": false, \"count\": 3}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *dup_enabled = cJSON_Duplicate(enabled_item, 1);
    cJSON_AddItemToObject(root, "enabled_copy", dup_enabled);

    // step 3: Operate
    cJSON_bool was_false = cJSON_IsFalse(flag_item);
    cJSON *flag_was_false_item = cJSON_AddBoolToObject(root, "flag_was_false", was_false);
    char *out_root = cJSON_PrintBuffered(root, 128, 1);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    char *out_root_dup = cJSON_PrintBuffered(root_dup, 64, 0);

    // step 4: Validate & Cleanup
    cJSON_free(out_root);
    cJSON_free(out_root_dup);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}