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
//<ID> 259
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 0, n1);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, n2);
    cJSON *btrue = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, btrue);
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, bfalse);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(arr);
    cJSON *s = cJSON_CreateString("hello");
    cJSON_InsertItemInArray(arr, 1, s);
    int size_after = cJSON_GetArraySize(arr);

    // step 3: Operate and Validate
    cJSON_bool is_btrue_bool = cJSON_IsBool(btrue);
    cJSON_bool is_btrue_true = cJSON_IsTrue(btrue);
    cJSON_bool is_bfalse_true = cJSON_IsTrue(bfalse);
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    cJSON_AddBoolToObject(root, "btrue_is_bool", is_btrue_bool);
    cJSON_AddBoolToObject(root, "btrue_is_true", is_btrue_true);
    cJSON_AddBoolToObject(root, "bfalse_is_true", is_bfalse_true);
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