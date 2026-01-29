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
//<ID> 305
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
    cJSON_AddNumberToObject(config, "pi", 3.1415);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *enabled_flag = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "enabled", enabled_flag);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "version", 2.0);

    // step 3: Operate and Validate
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *pi_item = cJSON_GetObjectItem(cfg, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON_AddNumberToObject(root, "pi_times_two", pi_val * 2.0);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *formatted = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(formatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}