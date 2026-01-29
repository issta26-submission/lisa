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
//<ID> 139
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
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *name = cJSON_CreateString("alpha");
    cJSON *replacement = cJSON_CreateString("beta");
    cJSON *count_item = NULL;
    cJSON *name_item = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double count_value = 0.0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddNumberToObject(root, "count", 42.0);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    count_item = cJSON_GetObjectItem(root, "count");
    count_value = cJSON_GetNumberValue(count_item);
    name_item = cJSON_GetObjectItem(root, "name");
    replaced = cJSON_ReplaceItemViaPointer(root, name_item, replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}