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
//<ID> 827
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON *raw = cJSON_CreateRaw("{\"note\":\"raw_content\"}");
    cJSON_AddItemToObject(root, "meta_raw", raw);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_missing", nul);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *approx = cJSON_CreateNumber(22.0 / 7.0);
    cJSON_AddItemToObject(meta, "approx_pi", approx);

    // step 3: Operate & Validate
    cJSON_bool is_null = cJSON_IsNull(nul);
    cJSON_bool is_raw = cJSON_IsRaw(raw);
    (void)is_null;
    (void)is_raw;
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_approx = cJSON_GetObjectItem(parsed_meta, "approx_pi");
    double approx_val = cJSON_GetNumberValue(parsed_approx);
    (void)approx_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}