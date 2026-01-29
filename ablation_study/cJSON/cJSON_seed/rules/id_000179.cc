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
//<ID> 179
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *name_item = cJSON_CreateString("original");
    cJSON *replacement = cJSON_CreateString("gamma");
    cJSON *compare_target = cJSON_CreateString("gamma");
    cJSON *current_name = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool equal = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON_AddItemToObject(root, "name", name_item);

    // step 3: Operate / Validate
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "name", replacement);
    current_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    equal = cJSON_Compare(current_name, compare_target, 1);
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "letters"));

    // step 4: Cleanup
    cJSON_Delete(compare_target);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)replaced;
    (void)equal;
    (void)arr_size;
    (void)current_name;
    return 66;
}