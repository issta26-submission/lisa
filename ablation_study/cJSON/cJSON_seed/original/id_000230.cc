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
//<ID> 230
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
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *pi = cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 2: Configure
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(arr, 0);
    int arr_size = cJSON_GetArraySize(arr);
    double second_value = cJSON_GetNumberValue(n2);
    char *printed = cJSON_Print(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = arr_size + (int)second_value + (int)flag_is_bool + (printed ? (int)printed[0] : 0);
    (void)summary;
    (void)pi;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}