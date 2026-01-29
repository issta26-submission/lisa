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
//<ID> 1422
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

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "values", array);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", label);

    // step 3: Operate & Validate
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    cJSON *dup_first = cJSON_Duplicate(cJSON_GetArrayItem(array, 0), 1);
    cJSON_AddItemToArray(array, dup_first);
    cJSON_bool equal_dup = cJSON_Compare(cJSON_GetArrayItem(array, 0), cJSON_GetArrayItem(array, 2), 1);
    cJSON_bool is_invalid_name = cJSON_IsInvalid(cJSON_GetObjectItem(root, "name"));
    double dup_val = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 2));
    double total = first_val + dup_val + (equal_dup ? 0.0 : 1.0) + (is_invalid_name ? 0.0 : 0.0);
    cJSON_AddNumberToObject(root, "computed_sum", total);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}