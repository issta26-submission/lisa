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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddNumberToObject(root, "num", 42.0);
    cJSON_AddFalseToObject(root, "flag");
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(30));
    int size_before = cJSON_GetArraySize(arr);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", true_item);

    // step 3: Operate and Validate
    cJSON *flag_item = cJSON_GetObjectItemCaseSensitive(root, "flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(flag_item);
    cJSON_DeleteItemFromArray(arr, 1);
    int size_after = cJSON_GetArraySize(arr);
    char *serialized = cJSON_Print(root);

    // step 4: Cleanup
    (void)version;
    (void)replaced;
    (void)flag_is_true;
    (void)size_before;
    (void)size_after;
    cJSON_free(serialized);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}