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
//<ID> 1190
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
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(7.0);
    cJSON *explicit_true = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "explicitTrue", explicit_true);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateTrue();
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool are_equal = cJSON_Compare(second, explicit_true, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)are_equal;
    return 66;
}