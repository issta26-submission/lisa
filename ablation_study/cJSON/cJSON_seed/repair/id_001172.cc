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
//<ID> 1172
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
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *label = cJSON_AddStringToObject(root, "label", "initial");
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddNumberToObject(root, "version", 1.5);
    char *setret = cJSON_SetValuestring(label, "updated");
    (void)setret;

    // step 3: Operate and Validate
    double version = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "version"));
    cJSON_bool is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "maybe"));
    double final_val = version + (double)is_null + 42.0;
    cJSON_AddNumberToObject(root, "final", final_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}