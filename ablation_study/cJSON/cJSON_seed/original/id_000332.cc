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
//<ID> 332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[3] = {10, 20, 30};
    cJSON *intArr = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObjectCS(root, "ints", intArr);

    // step 2: Configure
    cJSON *placeholder = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "to_replace", placeholder);
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddNumberToObject(replacement, "initial", 0.0);
    cJSON_ReplaceItemViaPointer(root, placeholder, replacement);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(intArr);
    cJSON *first = cJSON_GetArrayItem(intArr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(replacement, "first_value", first_val);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddBoolToObject(root, "prealloc_ok", prealloc_ok);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}