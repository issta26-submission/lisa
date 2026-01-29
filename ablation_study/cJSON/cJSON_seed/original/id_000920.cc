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
//<ID> 920
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"active\": false, \"count\": 3}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON_bool require_null_terminated = 1;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, require_null_terminated);

    // step 2: Configure
    cJSON *added_flag = cJSON_AddBoolToObject(root, "new_flag", 1);
    (void)added_flag;
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool was_false = cJSON_IsFalse(active_item);
    cJSON *added_false_marker = cJSON_AddBoolToObject(root, "active_was_false", was_false);
    (void)added_false_marker;

    // step 3: Operate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    char *printed = cJSON_PrintBuffered(dup_root, 64, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}