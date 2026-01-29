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
//<ID> 927
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
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool was_active_false = cJSON_IsFalse(active_item);
    cJSON *added_enabled = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *out = cJSON_PrintBuffered(dup, 128, 1);
    size_t parsed_consumed = (size_t)(parse_end - initial_json);
    (void)was_active_false;
    (void)parsed_consumed;
    (void)added_enabled;

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}