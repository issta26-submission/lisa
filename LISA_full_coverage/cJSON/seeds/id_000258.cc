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
//<ID> 258
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
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *n1 = cJSON_CreateNumber(1.234);
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, bfalse);
    cJSON *btrue = cJSON_CreateTrue();
    cJSON_InsertItemInArray(arr, 1, btrue);

    // step 2: Configure
    cJSON_AddStringToObject(root, "info", "example");
    cJSON_AddNumberToObject(root, "first_value", cJSON_GetNumberValue(n1));

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "size", (double)size);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool item1_true = cJSON_IsTrue(item1);
    cJSON_AddBoolToObject(root, "item1_is_true", item1_true);
    cJSON_bool bfalse_is_bool = cJSON_IsBool(bfalse);
    cJSON_AddBoolToObject(root, "bfalse_is_bool", bfalse_is_bool);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    int summary = size + (int)item1_true + (int)bfalse_is_bool + (int)printed[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}