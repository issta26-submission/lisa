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
//<ID> 1574
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
    cJSON *pi_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(config, "pi", pi_item);
    cJSON *active_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "active", active_item);
    const char *json_text = "{\"meta\":{\"version\":2}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON *version_item = cJSON_GetObjectItem(meta, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    cJSON *version_num = cJSON_CreateNumber(version_val);
    cJSON_AddItemToObject(root, "version", version_num);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = strlen(out);
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)meta_is_obj);
    scratch[1] = out[0];
    double pi_val = cJSON_GetNumberValue(cJSON_GetObjectItem(config, "pi"));
    int pi_int = (int)pi_val;
    scratch[2] = (char)('0' + (pi_int % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}