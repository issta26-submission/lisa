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
//<ID> 655
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
    cJSON *status = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(root, "status", status);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 12.5);
    cJSON_AddNumberToObject(config, "max_retries", 3.0);
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObjectCS(config, "name", name);

    // step 3: Operate and Validate
    cJSON_bool has_threshold = cJSON_HasObjectItem(config, "threshold");
    cJSON_AddBoolToObject(root, "has_threshold", has_threshold);
    cJSON *thr_item = cJSON_GetObjectItem(config, "threshold");
    double thr_val = cJSON_GetNumberValue(thr_item);
    cJSON_AddNumberToObject(root, "threshold_readback", thr_val);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}