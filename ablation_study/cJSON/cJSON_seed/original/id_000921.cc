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
//<ID> 921
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"flag\": false, \"value\": 123}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_was_false = cJSON_IsFalse(flag_item);
    cJSON_AddBoolToObject(root, "flag_was_false", flag_was_false);

    // step 3: Operate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_AddBoolToObject(dup, "duplicate_marker", 0);
    cJSON_AddBoolToObject(dup, "source_was_false", cJSON_IsFalse(flag_item));
    char *buf = cJSON_PrintBuffered(dup, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(buf);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}