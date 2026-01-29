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
//<ID> 714
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    int nums[] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(nums, 4);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "nums", int_array);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(int_array, 2);
    cJSON_AddItemToObjectCS(root, "removed", detached);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}