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
//<ID> 542
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
    int nums[4] = {1, 2, 3, 4};
    cJSON *int_array = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "numbers", int_array);

    // step 2: Configure
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 4.0);
    cJSON *sum_item = cJSON_AddNumberToObject(root, "sum", 10.0);

    // step 3: Operate & Validate
    cJSON *detached_count = cJSON_DetachItemFromObjectCaseSensitive(root, "count");
    double detached_value = cJSON_GetNumberValue(detached_count);
    cJSON *doubled_item = cJSON_AddNumberToObject(root, "doubled", detached_value * 2.0);
    char *snapshot = cJSON_PrintUnformatted(root);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;sum=%.0f;detached=%.0f",
            version ? version : "null",
            cJSON_GetNumberValue(sum_item),
            detached_value);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(detached_count);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}