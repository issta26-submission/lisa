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
//<ID> 2198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *nA = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, nA);
    cJSON *nB = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, nB);
    cJSON *nC = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, nC);
    int nums[3] = {100, 200, 300};
    cJSON *intarr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "intarr", intarr);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "label", label);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(items, 1);
    cJSON_bool inserted = cJSON_InsertItemInArray(intarr, 1, detached);
    (void)inserted;
    cJSON *first_int = cJSON_GetArrayItem(intarr, 0);
    double base = cJSON_GetNumberValue(first_int);
    cJSON *derived = cJSON_CreateNumber(base + 0.5);
    cJSON_AddItemToObject(meta, "derived", derived);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
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