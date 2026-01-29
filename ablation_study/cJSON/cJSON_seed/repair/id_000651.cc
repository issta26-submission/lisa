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
//<ID> 651
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
    cJSON *threshold = cJSON_AddNumberToObject(config, "threshold", 12.5);
    cJSON *limit = cJSON_AddNumberToObject(config, "limit", 100.0);
    (void)threshold;
    (void)limit;

    // step 3: Operate and Validate
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON_AddNumberToObject(root, "config_present", (double)has_config);
    cJSON_bool has_status = cJSON_HasObjectItem(root, "status");
    cJSON_AddNumberToObject(root, "status_present", (double)has_status);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}