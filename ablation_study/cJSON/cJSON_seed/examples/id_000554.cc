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
//<ID> 554
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *threshold = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *opt = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "optional", opt);
    cJSON *label = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(config, "label", label);
    cJSON *enabled = cJSON_AddBoolToObject(config, "enabled", 1);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_config = cJSON_GetObjectItem(parsed, "config");
    cJSON *parsed_label = cJSON_GetObjectItem(parsed_config, "label");
    char *label_val = cJSON_GetStringValue(parsed_label);
    cJSON *parsed_threshold = cJSON_GetObjectItem(parsed_config, "threshold");
    double thr_val = cJSON_GetNumberValue(parsed_threshold);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;label=%s;thr=%.2f",
            version ? version : "null",
            label_val ? label_val : "null",
            thr_val);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}