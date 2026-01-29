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
//<ID> 719
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"numbers\": [10,20,30], \"meta\": {\"name\":\"test\"}}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");

    // step 2: Configure
    int new_vals[4] = {1, 2, 3, 4};
    cJSON *ints = cJSON_CreateIntArray(new_vals, 4);
    cJSON_AddItemToObjectCS(root, "ints", ints);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(numbers, 1);
    cJSON *holder = cJSON_CreateArray();
    cJSON_AddItemToArray(holder, detached);
    cJSON_AddItemToObjectCS(root, "detached_items", holder);
    int numbers_size_after = cJSON_GetArraySize(numbers);
    cJSON_AddNumberToObject(root, "numbers_size_after_detach", (double)numbers_size_after);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}