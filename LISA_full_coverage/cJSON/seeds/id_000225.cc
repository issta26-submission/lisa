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
//<ID> 225
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *bf = cJSON_CreateFalse();
    cJSON *bt = cJSON_CreateTrue();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *s = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, bf);
    cJSON_AddItemToArray(arr, bt);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, s);

    // step 2: Configure
    cJSON_AddItemReferenceToArray(arr, num);
    cJSON *detached = cJSON_DetachItemFromArray(arr, 2);
    cJSON_AddItemToObject(root, "detached_num", detached);
    cJSON *flag_item = cJSON_AddBoolToObject(root, "flag", 0);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    double det_value = cJSON_GetNumberValue(detached);
    int is_false_flag = cJSON_IsFalse(flag_item);
    int is_false_bf = cJSON_IsFalse(bf);
    int summary = arr_size + (int)det_value + is_false_flag + is_false_bf;
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}