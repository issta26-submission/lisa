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
//<ID> 1132
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
    cJSON *ref_item = NULL;
    cJSON *replacement = NULL;
    cJSON *old_meta = NULL;
    cJSON *new_meta = NULL;
    cJSON *detached_ref = NULL;
    cJSON *first_elem = NULL;
    const char *first_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_a = cJSON_CreateString("alpha");
    item_b = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_b);
    cJSON_AddItemToObject(root, "items", arr);
    old_meta = cJSON_CreateString("meta_old");
    cJSON_AddItemToObject(root, "meta", old_meta);
    ref_item = cJSON_CreateString("external_ref");
    cJSON_AddItemReferenceToArray(arr, ref_item);

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("beta_replaced");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetStringValue(first_elem);
    new_meta = cJSON_CreateString(first_val); // propagate value into new meta
    cJSON_ReplaceItemInObjectCaseSensitive(root, "meta", new_meta);
    detached_ref = cJSON_DetachItemFromArray(arr, 2);

    // step 4: Cleanup
    cJSON_Delete(detached_ref);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}