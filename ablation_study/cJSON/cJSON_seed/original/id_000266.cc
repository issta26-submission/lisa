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
//<ID> 266
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *child = cJSON_CreateObject();
    cJSON *n = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "value", n);
    cJSON_AddFalseToObject(child, "enabled");
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(arr, child_ref);
    cJSON *extra_num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(arr, extra_num);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    double original = cJSON_GetNumberValue(n);
    cJSON_AddNumberToObject(root, "array_size", (double)size);
    cJSON_AddNumberToObject(root, "original_plus_one", original + 1.0);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}