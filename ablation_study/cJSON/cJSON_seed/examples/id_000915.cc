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
//<ID> 915
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(root, str_item);
    cJSON_AddItemToArray(root, true_item);
    cJSON_AddItemToArray(root, num_item);
    cJSON_SetNumberHelper(num_item, 3.1415);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(root, 0);
    cJSON *compare_str = cJSON_CreateString("alpha");
    cJSON_bool equal = cJSON_Compare(first_item, compare_str, 1);
    cJSON *num_from_array = cJSON_GetArrayItem(root, 2);
    double current = cJSON_GetNumberValue(num_from_array);
    cJSON_SetNumberHelper(num_from_array, current + 1.0);
    (void)equal;
    (void)current;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(compare_str);
    // API sequence test completed successfully
    return 66;
}