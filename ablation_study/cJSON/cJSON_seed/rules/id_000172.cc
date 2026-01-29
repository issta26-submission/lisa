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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *arr_item = cJSON_CreateString("alpha");
    cJSON *obj_item = cJSON_CreateString("alpha");
    cJSON *replacement = cJSON_CreateString("gamma");
    cJSON *old_item = NULL;
    cJSON *new_item = NULL;
    cJSON_bool compared_before = 0;
    cJSON_bool replace_result = 0;
    cJSON_bool compared_after = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, arr_item);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "name", obj_item);

    // step 3: Operate / Validate
    old_item = cJSON_GetObjectItem(root, "name");
    compared_before = cJSON_Compare(old_item, replacement, 1);
    replace_result = cJSON_ReplaceItemInObjectCaseSensitive(root, "name", replacement);
    new_item = cJSON_GetObjectItem(root, "name");
    compared_after = cJSON_Compare(new_item, replacement, 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)compared_before;
    (void)replace_result;
    (void)compared_after;
    (void)old_item;
    (void)new_item;
    return 66;
}