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
//<ID> 654
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
    cJSON *status = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(config, "enabled", status);
    cJSON_AddNumberToObject(config, "threshold", 42.0);

    // step 3: Operate and Validate
    cJSON_bool enabled_present = cJSON_HasObjectItem(config, "enabled");
    cJSON_AddNumberToObject(root, "enabled_present", (double)enabled_present);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(root, "snapshot", snapshot);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}