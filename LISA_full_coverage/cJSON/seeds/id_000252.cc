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
//<ID> 252
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("hello");
    cJSON *fls = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToArray(arr, fls);

    // step 2: Configure
    cJSON *ins = cJSON_CreateTrue();
    cJSON_InsertItemInArray(arr, 1, ins);
    cJSON_bool ins_is_true = cJSON_IsTrue(ins);
    cJSON_bool fls_is_bool = cJSON_IsBool(fls);
    cJSON_AddBoolToObject(root, "inserted_true", ins_is_true);
    cJSON_AddBoolToObject(root, "third_is_bool", fls_is_bool);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "size", (double)arr_size);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = (int)ins_is_true + (int)fls_is_bool + arr_size;
    cJSON_AddNumberToObject(root, "summary", (double)summary);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}