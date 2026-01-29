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
//<ID> 1018
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *dbl_array = NULL;
    cJSON *raw = NULL;
    cJSON *true_item = NULL;
    cJSON *old_item = NULL;
    cJSON *new_item = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    double nums[3] = {1.1, 2.2, 3.3};
    double val1 = 0.0;
    cJSON_bool was_replaced = 0;
    cJSON_bool flag_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "children", arr);
    dbl_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "doubles", dbl_array);
    raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToObject(root, "meta", raw);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);
    old_item = cJSON_CreateNumber(99.0);
    cJSON_AddItemToObject(root, "replace_me", old_item);
    new_item = cJSON_CreateString("replaced");
    was_replaced = cJSON_ReplaceItemViaPointer(root, old_item, new_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, 1);
    flag_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(dbl_array, 1));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}