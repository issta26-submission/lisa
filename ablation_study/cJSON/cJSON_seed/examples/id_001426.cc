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
//<ID> 1426
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
    cJSON *num2 = cJSON_CreateNumber(3.1415);
    cJSON *str1 = cJSON_CreateString("example");
    cJSON *nul = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, str1);
    cJSON_AddItemToArray(array, nul);
    cJSON_AddItemToObject(root, "payload", array);
    cJSON_AddNumberToObject(root, "meta", 7.0);

    // step 3: Operate & Validate
    double first_value = cJSON_GetNumberValue(num1);
    cJSON *dup_num = cJSON_Duplicate(num1, 1);
    cJSON_AddItemToArray(array, dup_num);
    cJSON_bool are_equal = cJSON_Compare(num1, dup_num, 1);
    cJSON_bool is_invalid = cJSON_IsInvalid(nul);
    cJSON_AddNumberToObject(root, "first_value", first_value);
    cJSON_AddBoolToObject(root, "first_equals_dup", are_equal);
    cJSON_AddBoolToObject(root, "null_is_invalid", is_invalid);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}