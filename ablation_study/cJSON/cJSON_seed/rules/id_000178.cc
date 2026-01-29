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
//<ID> 178
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
    cJSON *elem1 = cJSON_CreateString("elem1");
    cJSON *elem2 = cJSON_CreateString("elem2");
    cJSON *name_item = cJSON_CreateString("original_name");
    cJSON *to_replace = cJSON_CreateString("will_be_replaced");
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_bool replaced = 0;
    cJSON_bool cmp_equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "to_replace", to_replace);

    // step 3: Operate / Validate
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "to_replace", replacement);
    cmp_equal = cJSON_Compare(replacement, cJSON_GetObjectItem(root, "to_replace"), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}