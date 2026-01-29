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
//<ID> 282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const double numbers[] = {1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *list = cJSON_CreateArray();

    // step 2: Configure
    cJSON *flag_item = cJSON_AddFalseToObject(root, "active");
    cJSON_AddItemToArray(list, double_array);
    cJSON_AddItemToObject(root, "values", list);

    // step 3: Operate and Validate
    cJSON_bool is_active_true = cJSON_IsTrue(flag_item);
    cJSON *active_num = cJSON_CreateNumber((double)is_active_true);
    cJSON_AddItemToObject(root, "active_numeric", active_num);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}