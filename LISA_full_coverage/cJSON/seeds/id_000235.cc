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
//<ID> 235
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddNumberToObject(root, "count", 10.5);
    cJSON *b = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", b);
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str1);

    // step 2: Configure
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num2);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddNumberToObject(nested, "inner", 42.0);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_bool flag_is_bool = cJSON_IsBool(b);
    int arr_size = cJSON_GetArraySize(arr);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = (int)flag_is_bool + arr_size + (int)first_val;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}