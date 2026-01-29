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
//<ID> 548
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
    int numbers[] = {10, 20, 30};
    cJSON *arr = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "expected_sum", 60.0);
    cJSON *extra_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "extra", extra_num);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "expected_sum");
    double detached_value = cJSON_GetNumberValue(detached);
    cJSON_AddItemToObject(root, "readded_sum", detached);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_elem);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;detached=%.0f;first=%.0f",
            version ? version : "null",
            detached_value,
            first_val);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}