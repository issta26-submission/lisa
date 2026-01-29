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
//<ID> 653
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
    cJSON_AddItemToObjectCS(root, "config", config);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 42.0);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(config, "enabled", enabled);
    cJSON_AddNumberToObject(root, "version_major", 1.0);

    // step 3: Operate and Validate
    cJSON *thr = cJSON_GetObjectItem(config, "threshold");
    double thr_val = cJSON_GetNumberValue(thr);
    cJSON_AddNumberToObject(root, "threshold_readback", thr_val);
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON_AddBoolToObject(root, "config_present", has_config);

    // step 4: Cleanup
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    cJSON_AddStringToObject(root, "snapshot", buffer);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}