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
//<ID> 335
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
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *scalar = cJSON_AddNumberToObject(root, "scalar", 10.0);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(9.9);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate & Validate
    cJSON *got_scalar = cJSON_GetObjectItem(root, "scalar");
    double scalar_val = cJSON_GetNumberValue(got_scalar);
    double replacement_val = cJSON_GetNumberValue(replacement);
    cJSON_AddNumberToObject(root, "sum", scalar_val + replacement_val);
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Cleanup
    (void)replaced;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}