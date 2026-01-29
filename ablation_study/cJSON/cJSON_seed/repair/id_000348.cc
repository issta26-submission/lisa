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
//<ID> 348
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
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *threshold = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "threshold", threshold);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON_bool enabled_bool = cJSON_IsTrue(enabled);
    cJSON *enabled_num = cJSON_CreateNumber((double)enabled_bool);
    cJSON_AddItemToObject(root, "enabled_num", enabled_num);
    char *name_val = cJSON_GetStringValue(name);
    cJSON *copied_name = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "copied_name", copied_name);

    // step 3: Operate and Validate
    cJSON *got_threshold = cJSON_GetObjectItem(root, "threshold");
    double thr_val = cJSON_GetNumberValue(got_threshold);
    cJSON *double_thr = cJSON_CreateNumber(thr_val * 2.0);
    cJSON_AddItemToObject(root, "double_threshold", double_thr);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}