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
//<ID> 1428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.14);

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "data", array);

    // step 3: Operate & Validate
    cJSON *dup_num = cJSON_Duplicate(num1, 1);
    cJSON_AddItemToArray(array, dup_num);
    cJSON *item0 = cJSON_GetArrayItem(array, 0);
    cJSON *item2 = cJSON_GetArrayItem(array, 2);
    double v0 = cJSON_GetNumberValue(item0);
    double v2 = cJSON_GetNumberValue(item2);
    cJSON_bool equal = cJSON_Compare(item0, item2, 1);
    cJSON_bool invalid_flag = cJSON_IsInvalid(item0);
    double sum = v0 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}