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
//<ID> 1036
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON *val = cJSON_CreateNumber(42.0);
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(metrics, "value", val);
    cJSON_AddItemToObject(metrics, "name", name);
    cJSON_AddItemToObject(metrics, "active", active);
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 2: Configure
    cJSON *metrics_copy = cJSON_CreateObject();
    cJSON *val2 = cJSON_CreateNumber(42.0);
    cJSON *name2 = cJSON_CreateString("sensor-A");
    cJSON *active2 = cJSON_CreateTrue();
    cJSON_AddItemToObject(metrics_copy, "value", val2);
    cJSON_AddItemToObject(metrics_copy, "name", name2);
    cJSON_AddItemToObject(metrics_copy, "active", active2);

    // step 3: Operate & Validate
    cJSON_bool same = cJSON_Compare(metrics, metrics_copy, 1);
    cJSON_bool active_flag = cJSON_IsTrue(active);
    cJSON_AddNumberToObject(root, "same_flag", (double)same);
    cJSON_AddNumberToObject(root, "active_flag", (double)active_flag);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(metrics_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}