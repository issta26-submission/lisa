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
//<ID> 1429
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
    cJSON *num2 = cJSON_CreateNumber(3.5);
    cJSON *str1 = cJSON_CreateString("value");

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, str1);
    cJSON_AddItemToObject(root, "items", array);

    // step 3: Operate & Validate
    cJSON *dup_num = cJSON_Duplicate(num1, 1);
    cJSON_AddItemToArray(array, dup_num);
    cJSON *detached = cJSON_DetachItemFromArray(array, 1);
    cJSON_bool was_invalid = cJSON_IsInvalid(detached);
    double extracted = cJSON_GetNumberValue(num1);
    cJSON_bool are_equal = cJSON_Compare(num1, dup_num, 1);
    (void)was_invalid;
    (void)extracted;
    (void)are_equal;
    cJSON_Delete(detached);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}