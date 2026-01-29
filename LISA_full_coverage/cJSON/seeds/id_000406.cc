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
//<ID> 406
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON *status = cJSON_AddStringToObject(root, "status", "initial");

    // step 2: Configure
    cJSON *null_item = cJSON_CreateNull();
    cJSON_bool null_is_invalid = cJSON_IsInvalid(null_item);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);

    // step 3: Operate
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *new_status = cJSON_CreateString("updated");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "status", new_status);
    cJSON_AddItemToObject(root, "backup", dup_arr);

    // step 4: Validate and Cleanup
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);
    (void)null_is_invalid;
    (void)replaced;
    (void)arr_size;
    (void)first_val;
    cJSON_free(snapshot);
    cJSON_Delete(null_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}