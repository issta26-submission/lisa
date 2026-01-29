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
//<ID> 928
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"flag\": false, \"nested\": {\"inner\": true}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_AddBoolToObject(dup, "added_flag", 1);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_was_false = cJSON_IsFalse(flag_item);
    cJSON_AddBoolToObject(root, "was_false", flag_was_false);

    // step 3: Operate
    cJSON *nested = cJSON_GetObjectItem(root, "nested");
    cJSON *inner = cJSON_GetObjectItem(nested, "inner");
    cJSON_bool inner_is_false = cJSON_IsFalse(inner);
    cJSON_AddBoolToObject(dup, "inner_is_false", inner_is_false);
    char *out_root = cJSON_PrintBuffered(root, 256, 1);
    char *out_dup = cJSON_PrintBuffered(dup, 128, 0);

    // step 4: Validate & Cleanup
    void *tmpbuf = cJSON_malloc(64);
    memset(tmpbuf, 0, 64);
    cJSON_free(out_root);
    cJSON_free(out_dup);
    cJSON_free(tmpbuf);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}