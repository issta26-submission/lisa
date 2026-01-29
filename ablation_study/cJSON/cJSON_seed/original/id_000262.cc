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
//<ID> 262
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *num1 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, num1);

    // step 2: Configure
    /* add a reference to the existing num1 in the array (no separate reference object) */
    cJSON_AddItemReferenceToArray(arr, num1);
    cJSON *false_item = cJSON_AddFalseToObject(root, "flag");
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num2);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(arr);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    double v1 = cJSON_GetNumberValue(num1);
    double v2 = cJSON_GetNumberValue(num2);
    int summary = size_before + (int)v1 + (int)v2 + (int)printed[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}