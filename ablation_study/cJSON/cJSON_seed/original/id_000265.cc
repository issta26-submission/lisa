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
//<ID> 265
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
    cJSON *num1 = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(child, "value", num1);
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(arr, ref);
    cJSON *num2 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num2);
    cJSON *flag = cJSON_AddFalseToObject(root, "enabled");

    // step 2: Configure
    int size_before = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON *num3 = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(arr, num3);
    int size_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    cJSON_Minify(printed);
    int final_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "final_size_reported", (double)final_size);
    char *printed2 = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(child);

    // API sequence test completed successfully
    return 66;
}