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
//<ID> 135
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
    cJSON *item1 = cJSON_CreateString("first");
    cJSON *item2 = cJSON_CreateString("second");
    cJSON *item3 = cJSON_CreateString("third");
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON *to_replace = NULL;
    char *printed = NULL;
    int arr_size_before = 0;
    int arr_size_after = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToArray(arr, item3);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate / Validate
    arr_size_before = cJSON_GetArraySize(arr);
    to_replace = cJSON_GetArrayItem(arr, 1);
    replaced = cJSON_ReplaceItemViaPointer(arr, to_replace, replacement);
    arr_size_after = cJSON_GetArraySize(arr);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}