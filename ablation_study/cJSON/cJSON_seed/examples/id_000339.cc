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
//<ID> 339
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
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "direct", 7.25);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 0, replacement);

    // step 3: Operate & Validate
    cJSON *values_obj = cJSON_GetObjectItem(root, "values");
    cJSON *first_elem = cJSON_GetArrayItem(values_obj, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *direct_elem = cJSON_GetObjectItem(root, "direct");
    double direct_val = cJSON_GetNumberValue(direct_elem);
    double sum = first_val + direct_val;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}