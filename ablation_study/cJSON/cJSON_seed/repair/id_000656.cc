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
//<ID> 656
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
    cJSON *status_false = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(root, "status", status_false);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 3.14);
    cJSON *mode = cJSON_CreateString("auto");
    cJSON_AddItemToObjectCS(config, "mode", mode);
    cJSON_AddNumberToObject(root, "version_major", 1.0);

    // step 3: Operate and Validate
    cJSON_bool has_threshold = cJSON_HasObjectItem(config, "threshold");
    cJSON_AddNumberToObject(root, "threshold_present", (double)has_threshold);
    cJSON *th_item = cJSON_GetObjectItem(config, "threshold");
    double th_value = cJSON_GetNumberValue(th_item);
    cJSON_AddNumberToObject(root, "threshold_value", th_value);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed_ok;
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}