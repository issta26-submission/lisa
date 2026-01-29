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
//<ID> 173
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
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON *to_replace = cJSON_CreateString("old");
    cJSON *replacement = cJSON_CreateString("new");
    cJSON *temp_alpha = cJSON_CreateString("alpha");
    cJSON *got_key = NULL;
    cJSON *first_in_arr = NULL;
    cJSON_bool replaced_ok = 0;
    cJSON_bool compare_ok = 0;
    cJSON_bool compare_first = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "key", to_replace);

    // step 3: Operate / Validate
    replaced_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "key", replacement);
    got_key = cJSON_GetObjectItemCaseSensitive(root, "key");
    compare_ok = cJSON_Compare(got_key, replacement, 1);
    first_in_arr = cJSON_GetArrayItem(arr, 0);
    compare_first = cJSON_Compare(first_in_arr, temp_alpha, 1);

    // step 4: Cleanup
    cJSON_Delete(temp_alpha);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)replaced_ok;
    (void)compare_ok;
    (void)compare_first;
    (void)got_key;
    (void)first_in_arr;
    return 66;
}