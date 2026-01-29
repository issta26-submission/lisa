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
//<ID> 1427
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
    cJSON *nul = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToArray(array, nul);
    cJSON_AddItemToObject(root, "values", array);

    // step 3: Operate & Validate
    cJSON *retrieved_array = cJSON_GetObjectItem(root, "values");
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_array, 0));
    cJSON *dup_array = cJSON_Duplicate(retrieved_array, 1);
    cJSON_bool are_equal = cJSON_Compare(retrieved_array, dup_array, 1);
    cJSON_bool is_third_invalid = cJSON_IsInvalid(dup_array->child->next->next);
    cJSON *newnum = cJSON_CreateNumber(first_val + 1.0);
    cJSON_AddItemToArray(retrieved_array, newnum);

    // step 4: Cleanup
    cJSON_Delete(dup_array);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}