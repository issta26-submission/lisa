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
//<ID> 259
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
    cJSON *str_ref = cJSON_CreateStringReference("static_string");
    cJSON *dyn_str = cJSON_CreateString("dynamic_string");
    int arr_size = 0;
    const char *first_value = NULL;
    cJSON *first_item = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str_ref);
    cJSON_AddItemToArray(arr, dyn_str);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetStringValue(first_item);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "count");
    (void)arr_size;
    (void)first_value;
    (void)first_item;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}