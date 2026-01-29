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
//<ID> 544
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);
    cJSON *greet = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(container, "greeting", greet);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromObject(container, "greeting");
    char *greet_val = cJSON_GetStringValue(detached);
    cJSON *copy = cJSON_CreateString(greet_val);
    cJSON_AddItemToArray(arr, detached);
    cJSON_AddItemToArray(arr, copy);

    // step 3: Operate and Validate
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *size_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "items_count", size_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}