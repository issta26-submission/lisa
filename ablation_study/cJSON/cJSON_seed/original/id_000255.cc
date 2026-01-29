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
//<ID> 255
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *b_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, b_true);
    cJSON *b_false = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_InsertItemInArray(arr, 1, b_false); // insert false at index 1
    int size = cJSON_GetArraySize(arr);

    // step 3: Operate and Validate
    cJSON *mid_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool mid_is_bool = cJSON_IsBool(mid_item);
    cJSON_bool mid_is_true = cJSON_IsTrue(mid_item);
    cJSON_bool true_is_true = cJSON_IsTrue(b_true);
    cJSON_AddNumberToObject(root, "array_size", (double)size);
    cJSON_AddNumberToObject(root, "mid_is_bool", (double)mid_is_bool);
    cJSON_AddNumberToObject(root, "mid_is_true", (double)mid_is_true);
    cJSON_AddNumberToObject(root, "true_is_true", (double)true_is_true);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    int summary = size + (int)mid_is_bool + (int)mid_is_true + (int)true_is_true;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}