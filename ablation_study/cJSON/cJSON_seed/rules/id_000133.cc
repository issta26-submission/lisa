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
//<ID> 133
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
    cJSON *replacement = cJSON_CreateString("gamma");
    cJSON *found = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddNumberToObject(root, "initial_count", 2.0);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "array_size", (double)arr_size);
    found = cJSON_GetObjectItem(root, "initial_count");
    replaced = cJSON_ReplaceItemViaPointer(root, found, replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}