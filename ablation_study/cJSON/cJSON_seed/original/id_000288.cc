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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *child = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToArray(child, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(child, cJSON_CreateNumber(2.0));
    cJSON *num_elem = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(arr, num_elem);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateArrayReference(child);
    cJSON_AddItemToArray(arr, child_ref);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.1415));

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first);
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    (void)first_is_bool;
    (void)printed;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}