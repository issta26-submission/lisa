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
//<ID> 544
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
    int nums_arr[3] = {1, 2, 3};
    cJSON *ints = cJSON_CreateIntArray(nums_arr, 3);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *threshold = cJSON_AddNumberToObject(root, "threshold", 3.14159);

    // step 2: Configure
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *detached_threshold = cJSON_DetachItemFromObjectCaseSensitive(root, "threshold");
    cJSON_AddItemToObject(config, "threshold", detached_threshold);

    // step 3: Operate & Validate
    double threshold_value = cJSON_GetNumberValue(detached_threshold);
    cJSON_AddNumberToObject(config, "double_threshold", threshold_value * 2.0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;threshold=%f", version ? version : "null", threshold_value);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}