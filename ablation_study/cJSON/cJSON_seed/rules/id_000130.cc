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
//<ID> 130
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
    cJSON *label = cJSON_CreateString("original");
    cJSON *replacement = cJSON_CreateString("replaced");
    char *printed = NULL;
    int array_size = 0;
    cJSON_bool replaced = 0;
    cJSON *found_label = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(arr);
    found_label = cJSON_GetObjectItem(root, "label");
    replaced = cJSON_ReplaceItemViaPointer(root, found_label, replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}