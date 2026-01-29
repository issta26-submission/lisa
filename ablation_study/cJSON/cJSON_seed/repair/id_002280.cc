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
//<ID> 2280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "active_flag", flag_false);
    cJSON *missing_node = cJSON_AddNullToObject(root, "optional_value");

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version_major", 1.0);
    cJSON_AddStringToObject(root, "device", "sensor-7");
    cJSON *config = cJSON_CreateObject();
    cJSON_AddNumberToObject(config, "threshold", 75.5);
    cJSON_AddItemToObject(root, "config", config);

    // step 3: Operate
    cJSON *got_missing = cJSON_GetObjectItem(root, "optional_value");
    cJSON_bool is_missing_null = cJSON_IsNull(got_missing);
    cJSON_bool is_flag_bool = cJSON_IsBool(flag_false);
    cJSON_bool is_flag_false = cJSON_IsFalse(flag_false);
    double summary = (double)(is_missing_null + is_flag_bool + is_flag_false);
    cJSON *summary_num = cJSON_CreateNumber(summary);
    cJSON_AddItemToObject(root, "integrity_score", summary_num);

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}