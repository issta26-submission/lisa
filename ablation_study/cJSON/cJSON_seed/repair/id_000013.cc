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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"title\":\"demo\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(arr, false_item);

    // step 3: Operate and Validate
    char *serialized = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_false = cJSON_IsFalse(first);
    cJSON_AddNumberToObject(root, "validation_sum", (double)(arr_size + (int)first_is_false + (serialized != NULL)));

    // step 4: Cleanup
    if (serialized) { cJSON_free(serialized); }
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}