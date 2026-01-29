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
//<ID> 320
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

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, s1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    double sum = cJSON_GetNumberValue(item0) + cJSON_GetNumberValue(item1);
    cJSON_AddNumberToObject(meta, "sum", sum);
    cJSON *dup_n1 = cJSON_Duplicate(n1, 1);
    cJSON_bool eq = cJSON_Compare(n1, dup_n1, 1);
    cJSON_AddBoolToObject(meta, "n1_equals_dup", eq);
    const char *extracted = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 2));
    cJSON_AddStringToObject(meta, "extracted", extracted);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddBoolToObject(meta, "printed_prealloc", pre_ok);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(dup_n1);
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}