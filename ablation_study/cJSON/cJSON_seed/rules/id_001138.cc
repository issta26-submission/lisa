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
//<ID> 1138
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
    cJSON *to_replace = NULL;
    cJSON *got0 = NULL;
    const char *got0_str = NULL;
    cJSON *new_item = NULL;
    cJSON *repl_obj = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_a = cJSON_CreateString("alpha");
    item_b = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_b);
    cJSON_AddItemToObject(root, "items", arr);
    to_replace = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "replace_me", to_replace);

    // step 3: Operate / Validate
    cJSON_AddItemReferenceToArray(arr, item_a);
    got0 = cJSON_GetArrayItem(arr, 0);
    got0_str = cJSON_GetStringValue(got0);
    new_item = cJSON_CreateString("gamma");
    cJSON_ReplaceItemInArray(arr, 1, new_item);
    repl_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(repl_obj, "replaced", 1.23);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", repl_obj);

    // step 4: Cleanup
    (void)got0_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}