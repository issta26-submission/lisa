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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num_pi = cJSON_CreateNumber(3.141592653589793);
    cJSON *num_flag = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(values, num_pi);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToObject(root, "flag", num_flag);

    // step 3: Operate & Validate
    double new_flag_value = cJSON_SetNumberHelper(num_flag, 1.0);
    cJSON *num_flag_copy = cJSON_CreateNumber(new_flag_value);
    cJSON_AddItemToArray(values, num_flag_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}