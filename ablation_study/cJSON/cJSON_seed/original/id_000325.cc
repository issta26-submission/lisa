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
//<ID> 325
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON *s = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, s);
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    cJSON *other = cJSON_CreateObject();
    cJSON *dup_n1 = cJSON_Duplicate(n1, 0);
    cJSON_AddItemToObject(other, "n1_copy", dup_n1);
    cJSON_AddItemToObject(root, "other", other);

    // step 3: Operate and Validate
    cJSON *arr_item1 = cJSON_GetArrayItem(arr, 1);
    double val1 = cJSON_GetNumberValue(arr_item1);
    cJSON_AddNumberToObject(other, "val_from_index_1", val1);
    cJSON *arr_item0 = cJSON_GetArrayItem(arr, 0);
    cJSON_bool eq = cJSON_Compare(dup_n1, arr_item0, 1);
    cJSON_AddBoolToObject(other, "dup_equals_original", eq);
    cJSON_DeleteItemFromArray(arr, 2);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, buffer, 512, 0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}