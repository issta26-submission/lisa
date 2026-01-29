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
//<ID> 107
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

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *str1 = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str1);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_InsertItemInArray(arr, 1, num2);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_Minify(printed);
    int arr_size = cJSON_GetArraySize(arr);
    double mid_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    int summary = arr_size + (int)mid_value + (int)printed[0];
    (void)summary;
    (void)flag;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}