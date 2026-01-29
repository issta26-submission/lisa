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
//<ID> 652
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_bool added_settings = cJSON_AddItemToObjectCS(root, "settings", settings);
    cJSON *status = cJSON_CreateFalse();
    cJSON_bool added_status = cJSON_AddItemToObjectCS(root, "status", status);
    cJSON *threshold_item = cJSON_AddNumberToObject(settings, "threshold", 12.5);

    // step 2: Configure
    cJSON *limit_item = cJSON_AddNumberToObject(settings, "limit", 100.0);
    cJSON_bool has_threshold = cJSON_HasObjectItem(settings, "threshold");
    cJSON *present_flag = cJSON_AddNumberToObject(root, "threshold_present", (double)has_threshold);

    // step 3: Operate and Validate
    double thresh_value = cJSON_GetNumberValue(threshold_item);
    cJSON *readback = cJSON_AddNumberToObject(root, "threshold_readback", thresh_value);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    (void)printed_ok;
    cJSON_AddStringToObject(root, "snapshot", buffer);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}