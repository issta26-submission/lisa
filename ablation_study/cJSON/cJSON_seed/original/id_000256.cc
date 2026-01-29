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
//<ID> 256
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
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *b_true = cJSON_CreateTrue();
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_InsertItemInArray(arr, 0, n0);
    cJSON_InsertItemInArray(arr, 1, b_true);
    cJSON_InsertItemInArray(arr, 1, n1);

    // step 2: Configure
    int size_after_inserts = cJSON_GetArraySize(arr);
    cJSON_bool b_is_bool = cJSON_IsBool(b_true);
    cJSON_bool b_is_true = cJSON_IsTrue(b_true);
    cJSON_AddNumberToObject(root, "array_size", (double)size_after_inserts);
    cJSON_AddBoolToObject(root, "bool_is_bool", b_is_bool);
    cJSON_AddBoolToObject(root, "bool_is_true", b_is_true);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    int summary = size_after_inserts + (int)b_is_bool + (int)b_is_true;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}