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
//<ID> 2199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[3] = {10, 20, 30};
    cJSON *arrayA = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "A", arrayA);
    cJSON *arrayB = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "B", arrayB);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(arrayA, 0);
    cJSON *insert_item = cJSON_CreateNumber(25.0);
    cJSON_InsertItemInArray(arrayA, 1, insert_item);
    cJSON_AddItemToArray(arrayB, detached);

    // step 3: Operate and Validate
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}