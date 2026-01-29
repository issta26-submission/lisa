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
//<ID> 2194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *ints = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extras", extras);

    // step 2: Configure
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_InsertItemInArray(ints, 1, newnum);
    cJSON *detached = cJSON_DetachItemFromArray(ints, 2);
    cJSON_InsertItemInArray(ints, 0, detached);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(ints, 0);
    cJSON *second = cJSON_GetArrayItem(ints, 1);
    double sum = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "sum_first_two", sum);
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