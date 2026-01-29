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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version_str = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", version_str);
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *status = cJSON_CreateString("pending");
    cJSON_AddItemToObject(root, "status", status);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "status", true_item);

    // step 3: Operate and Validate
    cJSON *status_after = cJSON_GetObjectItem(root, "status");
    cJSON_bool is_true = cJSON_IsTrue(status_after);
    int size_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 0);
    int size_after = cJSON_GetArraySize(arr);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    double first_value = cJSON_GetNumberValue(first_elem);

    // step 4: Cleanup
    (void)version_str;
    (void)replaced;
    (void)is_true;
    (void)size_before;
    (void)size_after;
    (void)first_value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}