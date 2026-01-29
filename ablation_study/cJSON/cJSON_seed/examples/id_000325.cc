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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, nul);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *temp = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    cJSON_SetNumberHelper(num, 3.1415);
    cJSON_SetNumberHelper(temp, 100.5);
    cJSON_AddNumberToObject(root, "extra", 7.0);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *first_copy = cJSON_CreateNumber(first_val + 1.0);
    cJSON_AddItemToObject(root, "first_plus_one", first_copy);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}