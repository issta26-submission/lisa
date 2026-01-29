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
//<ID> 1378
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *retrieved = NULL;
    cJSON *flag = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double extracted_value = 0.0;
    cJSON_bool compare_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddNumberToObject(root, "count", 2.0);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 1);
    extracted_value = cJSON_GetNumberValue(retrieved);
    cJSON_AddNumberToObject(root, "extracted", extracted_value);
    compare_result = cJSON_Compare(retrieved, flag, 1);
    cJSON_AddNumberToObject(root, "cmp", (double)compare_result);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}