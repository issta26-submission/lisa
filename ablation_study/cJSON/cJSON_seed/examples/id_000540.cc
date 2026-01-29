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
//<ID> 540
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    int numbers[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *threshold = cJSON_AddNumberToObject(root, "threshold", 25.5);

    // step 2: Configure
    cJSON *holder = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "holder", holder);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "threshold");
    cJSON_AddItemToObject(holder, "moved_threshold", detached);

    // step 3: Operate & Validate
    double moved_value = cJSON_GetNumberValue(detached);
    int arr_size = cJSON_GetArraySize(int_array);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;moved=%f;size=%d", version ? version : "null", moved_value, arr_size);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}