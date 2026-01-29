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
//<ID> 1423
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
    cJSON *num1 = cJSON_CreateNumber(3.14159);
    cJSON *num2 = cJSON_CreateNumber(2.71828);
    cJSON *text = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, text);
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddNumberToObject(root, "constant", 42.0);

    // step 3: Operate & Validate
    double v1 = cJSON_GetNumberValue(num1);
    double v2 = cJSON_GetNumberValue(num2);
    double total = v1 + v2;
    cJSON_AddNumberToObject(root, "sum", total);
    cJSON_bool was_invalid = cJSON_IsInvalid(num1);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, copy, 1);

    // step 4: Cleanup
    cJSON_Delete(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}