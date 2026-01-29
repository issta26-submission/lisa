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
//<ID> 1571
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
    cJSON *enabled_item = cJSON_CreateTrue();
    cJSON *threshold_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(config, "enabled", enabled_item);
    cJSON_AddItemToObject(config, "threshold", threshold_item);

    // step 2: Configure
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON_bool config_is_obj = cJSON_IsObject(got_config);
    cJSON *got_enabled = cJSON_GetObjectItem(got_config, "enabled");
    cJSON *got_threshold = cJSON_GetObjectItem(got_config, "threshold");
    double threshold_val = cJSON_GetNumberValue(got_threshold);
    int threshold_int = (int)threshold_val;

    // step 3: Operate and Validate
    cJSON *new_threshold = cJSON_CreateNumber(threshold_val * 2.0);
    cJSON_ReplaceItemInObject(config, "threshold", new_threshold);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)config_is_obj);
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('0' + (threshold_int % 10));
    scratch[3] = (char)('0' + (int)(got_enabled->type));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}