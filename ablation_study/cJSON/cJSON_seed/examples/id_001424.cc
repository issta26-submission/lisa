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
//<ID> 1424
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
    cJSON *num_a = cJSON_CreateNumber(42.0);
    cJSON *num_b = cJSON_CreateNumber(3.14);
    cJSON *orphan = cJSON_CreateNumber(7.0);

    // step 2: Configure
    cJSON_AddItemToArray(array, num_a);
    cJSON_AddItemToArray(array, num_b);
    cJSON_AddItemToObject(root, "values", array);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *dup_first = cJSON_Duplicate(first, 1);
    cJSON_AddItemToArray(array, dup_first);
    cJSON_bool are_equal = cJSON_Compare(first, dup_first, 1);
    cJSON_bool is_invalid = cJSON_IsInvalid(first);
    double new_val = cJSON_SetNumberHelper(first, first_val + 10.0);
    cJSON_AddNumberToObject(root, "adjusted_first", new_val);
    (void)are_equal;
    (void)is_invalid;

    // step 4: Cleanup
    cJSON_Delete(orphan);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}