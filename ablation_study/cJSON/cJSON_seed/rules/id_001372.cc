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
//<ID> 1372
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *num_item = NULL;
    cJSON *flag = NULL;
    cJSON *flag_copy = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double extracted = 0.0;
    cJSON_bool cmp = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "top_number", 42.0);
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(10.0);
    num1 = cJSON_CreateNumber(20.0);
    num2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "items", arr);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    flag_copy = cJSON_CreateTrue();
    cmp = cJSON_Compare(flag, flag_copy, 1);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    num_item = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "second_value", extracted);
    printed = cJSON_PrintUnformatted(root);
    (void)arr_size;
    (void)cmp;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}