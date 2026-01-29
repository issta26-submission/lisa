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
//<ID> 1421
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
    cJSON *num2 = cJSON_CreateNumber(0.5);
    cJSON *s1 = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToObject(root, "list", array);

    // step 3: Operate & Validate
    double v1 = cJSON_GetNumberValue(num1);
    cJSON *num3 = cJSON_CreateNumber(v1 + cJSON_GetNumberValue(num2));
    cJSON_AddItemToArray(array, num3);
    cJSON *array_copy = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "list_copy", array_copy);
    cJSON_bool are_equal = cJSON_Compare(array, array_copy, 1);
    cJSON_bool is_invalid = cJSON_IsInvalid(num1);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)are_equal;
    (void)is_invalid;
    return 66;
}