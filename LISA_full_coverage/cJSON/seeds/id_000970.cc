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
//<ID> 970
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char raw_json[] = " { \"name\" : \"example\", \"value\" : 42, \"active\" : false, \"meta\": { \"count\": 1 } } ";
    int raw_len = (int)(sizeof(raw_json) - 1);
    char *minify_buf = (char *)cJSON_malloc((size_t)raw_len + 1);
    memcpy(minify_buf, raw_json, (size_t)raw_len);
    minify_buf[raw_len] = '\0';
    cJSON_Minify(minify_buf);
    cJSON *root = cJSON_Parse(minify_buf);

    // step 2: Configure
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional_null);
    cJSON *flag = cJSON_AddTrueToObject(root, "feature_enabled");

    // step 3: Operate
    char *unfmt = cJSON_PrintUnformatted(root);
    char *prebuf = (char *)cJSON_malloc(256);
    memset(prebuf, 0, 256);
    cJSON_PrintPreallocated(root, prebuf, 256, 1);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool are_equal = cJSON_Compare(root, duplicate, 1);
    cJSON_AddBoolToObject(root, "dup_equal", are_equal);

    // step 4: Validate & Cleanup
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    cJSON_free(unfmt);
    cJSON_free(prebuf);
    cJSON_free(minify_buf);

    // API sequence test completed successfully
    return 66;
}