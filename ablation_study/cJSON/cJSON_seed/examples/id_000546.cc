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
//<ID> 546
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

    // step 2: Configure
    int nums[4] = {10, 20, 30, 40};
    cJSON *ints_array = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", ints_array);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 4);

    // step 3: Operate & Validate
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *detached_ints = cJSON_DetachItemFromObjectCaseSensitive(root, "ints");
    cJSON *first_item = cJSON_GetArrayItem(detached_ints, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%.0f;first=%.0f",
            version ? version : "null",
            count_val,
            first_val);

    // step 4: Cleanup
    cJSON_Delete(detached_ints);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}