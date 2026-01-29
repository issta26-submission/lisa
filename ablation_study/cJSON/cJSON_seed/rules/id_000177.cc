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
//<ID> 177
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
    cJSON *str_a = cJSON_CreateString("alpha");
    cJSON *str_b = cJSON_CreateString("beta");
    cJSON *orig = cJSON_CreateString("original");
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON *got_key = NULL;
    cJSON *compare_tag = NULL;
    cJSON_bool replaced_ok = 0;
    cJSON_bool compare_equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, str_a);
    cJSON_AddItemToArray(arr, str_b);
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON_AddItemToObject(root, "key", orig);

    // step 3: Operate / Validate
    replaced_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "key", replacement);
    got_key = cJSON_GetObjectItemCaseSensitive(root, "key");
    compare_equal = cJSON_Compare(got_key, replacement, 1);
    compare_tag = cJSON_CreateString(compare_equal ? "equal" : "not_equal");
    cJSON_AddItemToObject(root, "compare_result", compare_tag);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}