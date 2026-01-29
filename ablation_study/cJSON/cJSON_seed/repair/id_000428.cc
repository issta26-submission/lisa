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
//<ID> 428
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
    cJSON *name = cJSON_CreateString("deviceA");
    cJSON_AddItemToObject(root, "device", name);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *threshold_item = cJSON_AddNumberToObject(config, "threshold", 3.5);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "scale", 2.25);
    cJSON *status = cJSON_CreateString("active");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "status", status);

    // step 3: Operate
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON_bool is_obj = cJSON_IsObject(config_ref);
    cJSON *thr = cJSON_GetObjectItem(config_ref, "threshold");
    double thr_val = cJSON_GetNumberValue(thr);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    double scale_val = cJSON_GetNumberValue(cJSON_GetObjectItem(meta_ref, "scale"));
    double scaled = thr_val * scale_val;
    cJSON *scaled_num = cJSON_CreateNumber(scaled);
    cJSON_AddItemToObject(root, "scaled_threshold", scaled_num);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    double retrieved = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "scaled_threshold"));
    (void)is_obj;
    (void)retrieved;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}