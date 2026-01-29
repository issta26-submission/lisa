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
//<ID> 972
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char raw_json[] = "{ \"name\": \"test\", \"values\": [1, 2, 3] }";
    size_t raw_len = sizeof(raw_json) - 1;
    char *json_buf = (char *)cJSON_malloc(raw_len + 1);
    memcpy(json_buf, raw_json, raw_len + 1);
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_Parse(json_buf);

    // step 2: Configure
    cJSON *active_item = cJSON_AddTrueToObject(root, "active");
    cJSON *missing_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", missing_null);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *meta_flag = cJSON_AddTrueToObject(meta, "flag");

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Validate & Cleanup
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_true = cJSON_IsTrue(got_active);
    cJSON *got_missing = cJSON_GetObjectItem(root, "missing");
    cJSON_bool missing_is_null = cJSON_IsNull(got_missing);
    (void)active_is_true;
    (void)missing_is_null;
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_free(json_buf);

    // API sequence test completed successfully
    return 66;
}