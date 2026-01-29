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
//<ID> 174
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
    cJSON *orig_name = cJSON_CreateString("Alice");
    cJSON *replacement_name = cJSON_CreateString("Bob");
    cJSON *array_name = cJSON_CreateString("Alice");
    cJSON_bool replaced = 0;
    cJSON_bool compare_after_replace = 0;
    cJSON_bool compare_replacement_vs_array = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "name", orig_name);
    cJSON_AddItemToObject(root, "people", arr);
    cJSON_AddItemToArray(arr, array_name);

    // step 3: Operate / Validate
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "name", replacement_name);
    compare_after_replace = cJSON_Compare(cJSON_GetObjectItem(root, "name"), replacement_name, 1);
    compare_replacement_vs_array = cJSON_Compare(replacement_name, cJSON_GetArrayItem(arr, 0), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)replaced;
    (void)compare_after_replace;
    (void)compare_replacement_vs_array;
    return 66;
}