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
//<ID> 1131
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
    cJSON *item_a = NULL;
    cJSON *item_b = NULL;
    cJSON *replacement_num = NULL;
    cJSON *entry_obj = NULL;
    cJSON *old_value = NULL;
    cJSON *new_value = NULL;
    cJSON *arr_item0 = NULL;
    cJSON *arr_item1 = NULL;
    const char *first_value = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_a = cJSON_CreateString("alpha");
    item_b = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_b);
    cJSON_AddItemReferenceToArray(arr, item_b);
    cJSON_AddItemToObject(root, "items", arr);
    entry_obj = cJSON_CreateObject();
    old_value = cJSON_CreateString("old");
    cJSON_AddItemToObject(root, "entry", entry_obj);
    cJSON_AddItemToObject(entry_obj, "value", old_value);

    // step 3: Operate / Validate
    arr_item0 = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetStringValue(arr_item0);
    replacement_num = cJSON_CreateNumber(3.14159);
    cJSON_ReplaceItemInArray(arr, 1, replacement_num);
    new_value = cJSON_CreateString("new");
    cJSON_ReplaceItemInObjectCaseSensitive(entry_obj, "value", new_value);
    arr_item1 = cJSON_GetArrayItem(arr, 1);
    (void)arr_item1;
    (void)first_value;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}