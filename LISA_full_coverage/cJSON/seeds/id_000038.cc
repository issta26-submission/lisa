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
//<ID> 38
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
    cJSON *array = cJSON_CreateArray();
    cJSON *arr_true = cJSON_CreateTrue();
    cJSON *arr_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, arr_true);
    cJSON_AddItemToArray(array, arr_num);
    cJSON_AddItemToObject(root, "arr", array);
    (void)version;

    // step 2: Configure
    cJSON *flag_initial = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_initial);
    cJSON *flag_replacement = cJSON_CreateTrue();
    cJSON_bool replace_result = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", flag_replacement);
    (void)replace_result;

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(array, 0);
    cJSON *flag_item = cJSON_GetObjectItemCaseSensitive(root, "flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(flag_item);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 0.0);
    double new_count = cJSON_SetNumberHelper(count_item, (double)flag_is_true);
    (void)new_count;
    (void)flag_is_true;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}