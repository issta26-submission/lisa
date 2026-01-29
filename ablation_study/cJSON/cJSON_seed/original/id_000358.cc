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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *num_item = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(items, true_item);
    cJSON_AddItemToArray(items, false_item);
    cJSON_AddItemToArray(items, num_item);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    int array_size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "array_size", (double)array_size);
    double flag_sum = (double)(cJSON_IsBool(true_item) + cJSON_IsTrue(true_item));
    cJSON_AddNumberToObject(root, "flag_sum", flag_sum);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddNumberToObject(root, "prealloc_ok", (double)prealloc_ok);

    // step 4: Validate and Cleanup
    cJSON_bool first_is_bool = cJSON_IsBool(cJSON_GetArrayItem(items, 0));
    cJSON_bool first_is_true = cJSON_IsTrue(cJSON_GetArrayItem(items, 0));
    (void)first_is_bool;
    (void)first_is_true;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}