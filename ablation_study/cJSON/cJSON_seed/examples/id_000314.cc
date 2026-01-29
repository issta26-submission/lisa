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
//<ID> 314
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    cJSON *false_item = cJSON_CreateFalse();
    int ints[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(ints, 4);

    // step 2: Configure
    cJSON_AddItemToArray(root, num_item);
    cJSON_AddItemToArray(root, false_item);
    cJSON_AddItemToArray(root, int_array);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(root, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *double_item = cJSON_CreateNumber(first_val * 2.0);
    cJSON_AddItemToArray(root, double_item);
    cJSON *third = cJSON_GetArrayItem(root, 2);
    cJSON *third_first = cJSON_GetArrayItem(third, 0);
    double third_first_val = cJSON_GetNumberValue(third_first);
    cJSON *derived = cJSON_CreateNumber(third_first_val + 0.5);
    cJSON_AddItemToArray(root, derived);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}