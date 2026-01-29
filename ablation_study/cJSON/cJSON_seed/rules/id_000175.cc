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
//<ID> 175
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
    cJSON *item_one = cJSON_CreateString("one");
    cJSON *item_two = cJSON_CreateString("two");
    cJSON *name_orig = cJSON_CreateString("original");
    cJSON *name_replacement = cJSON_CreateString("replaced");
    cJSON *retrieved_name_before = NULL;
    cJSON *retrieved_name_after = NULL;
    cJSON_bool compared_before = 0;
    cJSON_bool replaced = 0;
    cJSON_bool compared_after = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, item_one);
    cJSON_AddItemToArray(arr, item_two);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "name", name_orig);

    // step 3: Operate / Validate
    retrieved_name_before = cJSON_GetObjectItem(root, "name");
    compared_before = cJSON_Compare(retrieved_name_before, name_orig, 1);
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "name", name_replacement);
    retrieved_name_after = cJSON_GetObjectItem(root, "name");
    compared_after = cJSON_Compare(retrieved_name_after, name_replacement, 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}