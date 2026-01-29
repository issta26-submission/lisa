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
//<ID> 926
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"feature\": false, \"count\": 3}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *added_enabled = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *feature_item = cJSON_GetObjectItem(root, "feature");
    cJSON_bool feature_was_false = cJSON_IsFalse(feature_item);
    cJSON_bool fmt = (cJSON_bool)(1 - feature_was_false);

    // step 3: Operate
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *printed = cJSON_PrintBuffered(dup, 128, fmt);

    // step 4: Validate & Cleanup
    void *tmpbuf = cJSON_malloc(256);
    memset(tmpbuf, 0, 256);
    char *prebuf = (char *)tmpbuf;
    cJSON_free(prebuf);
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}