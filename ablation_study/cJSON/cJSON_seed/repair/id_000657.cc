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
//<ID> 657
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
    cJSON_AddItemToObject(root, "config", config);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(config, "enabled_cs", flag);
    cJSON *threshold_item = cJSON_AddNumberToObject(config, "threshold", 42.0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag_copy = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(meta, "enabled_cs_copy", flag_copy);
    cJSON_AddNumberToObject(meta, "threshold_copy", 100.5);

    // step 3: Operate and Validate
    cJSON_bool has_threshold = cJSON_HasObjectItem(config, "threshold");
    double present_val = (double)has_threshold;
    cJSON_AddNumberToObject(root, "threshold_present", present_val);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}