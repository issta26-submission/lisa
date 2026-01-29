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
//<ID> 263
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
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(child, "value", num);
    cJSON *flag = cJSON_AddFalseToObject(root, "flag");

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(arr, child_ref);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(arr);
    double extracted = cJSON_GetNumberValue(num);
    char *printed = cJSON_PrintUnformatted(root);
    int summary = size_before + (int)extracted + (int)printed[0];
    cJSON_AddNumberToObject(root, "array_size", (double)size_before);
    cJSON_AddNumberToObject(root, "extracted", extracted);
    cJSON_AddNumberToObject(root, "summary", (double)summary);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(child);

    // API sequence test completed successfully
    return 66;
}