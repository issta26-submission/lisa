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
//<ID> 2190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const int nums[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(nums, 4);
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "container", array);
    cJSON_InsertItemInArray(array, 0, int_array);
    cJSON *label = cJSON_CreateString("first_label");
    cJSON_AddItemToArray(array, label);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(array, 1);
    cJSON_InsertItemInArray(array, 0, detached);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(array);
    cJSON_AddNumberToObject(root, "count", (double)size);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON_AddStringToObject(root, "extracted", first_str ? first_str : "");
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}