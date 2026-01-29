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
//<ID> 132
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
    cJSON *arr_item1 = cJSON_CreateString("alpha");
    cJSON *arr_item2 = cJSON_CreateString("beta");
    cJSON *label = cJSON_CreateString("hello");
    cJSON *replacement = cJSON_CreateString("gamma");
    cJSON *num_added = NULL;
    cJSON *found = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double number_value = 0.0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, arr_item1);
    cJSON_AddItemToArray(arr, arr_item2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "label", label);
    num_added = cJSON_AddNumberToObject(root, "count", 7.5);

    // step 3: Operate / Validate
    found = cJSON_GetObjectItem(root, "label");
    replaced = cJSON_ReplaceItemViaPointer(root, found, replacement);
    arr_size = cJSON_GetArraySize(arr);
    number_value = cJSON_GetNumberValue(num_added);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}