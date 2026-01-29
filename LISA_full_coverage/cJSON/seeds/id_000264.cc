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
//<ID> 264
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
    cJSON *num1 = cJSON_CreateNumber(100.5);
    cJSON_AddItemToArray(arr, num1);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddFalseToObject(child, "flag");
    cJSON *inner_num = cJSON_CreateNumber(7.25);
    cJSON_AddItemToObject(child, "inner", inner_num);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(arr, child_ref);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(arr);
    cJSON *num2 = cJSON_CreateNumber((double)size_before * 2.0);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "first_value", first_value);
    cJSON_AddNumberToObject(root, "array_size", (double)size_before);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = (int)first_value + size_before + (int)printed[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(child);
    // API sequence test completed successfully
    return 66;
}