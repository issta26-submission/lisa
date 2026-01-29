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
//<ID> 1576
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *parsed = cJSON_Parse("{\"config\":{\"enabled\":true,\"threshold\":3.14}}");
    cJSON *config = cJSON_GetObjectItem(parsed, "config");
    cJSON_bool config_is_obj = cJSON_IsObject(config);

    // step 2: Configure
    cJSON *enabled_item = cJSON_GetObjectItem(config, "enabled");
    cJSON *threshold_item = cJSON_GetObjectItem(config, "threshold");
    double threshold_val = cJSON_GetNumberValue(threshold_item);
    cJSON *new_root = cJSON_CreateObject();
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(new_root, "flag", flag_item);
    cJSON *num_item = cJSON_CreateNumber(threshold_val);
    cJSON_AddItemToObject(new_root, "thresh", num_item);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(new_root, "thresh");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    cJSON_bool was_object = config_is_obj;
    char *out = cJSON_PrintUnformatted(new_root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)was_object);
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('0' + (int)retrieved_val);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(new_root);
    return 66;
    // API sequence test completed successfully
}