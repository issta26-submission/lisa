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
//<ID> 170
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
    cJSON *name_orig = cJSON_CreateString("original_name");
    cJSON *name_replacement = cJSON_CreateString("replaced_name");
    cJSON *arr_item1 = cJSON_CreateString("elem1");
    cJSON *arr_item2 = cJSON_CreateString("elem2");
    cJSON_bool replaced = 0;
    cJSON_bool equal_after_replace = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, arr_item1);
    cJSON_AddItemToArray(arr, arr_item2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "name", name_orig);

    // step 3: Operate / Validate
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "name", name_replacement);
    equal_after_replace = cJSON_Compare(cJSON_GetObjectItem(root, "name"), name_replacement, 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)replaced;
    (void)equal_after_replace;
    return 66;
}