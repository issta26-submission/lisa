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
//<ID> 913
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *str_a = cJSON_CreateString("Example");
    cJSON *str_b = cJSON_CreateString("example");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON *computed = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(root, str_a);
    cJSON_AddItemToArray(root, true_item);
    cJSON_AddItemToArray(root, num_item);
    cJSON_AddItemToArray(root, str_b);

    // step 3: Operate & Validate
    double set_val = cJSON_SetNumberHelper(num_item, 123.456);
    cJSON *retrieved_num = cJSON_GetArrayItem(root, 2);
    cJSON_bool strings_equal_ci = cJSON_Compare(cJSON_GetArrayItem(root, 0), cJSON_GetArrayItem(root, 3), 0);
    double final_val = cJSON_SetNumberHelper(computed, set_val + (double)strings_equal_ci);
    cJSON_AddItemToArray(root, computed);

    // step 4: Cleanup
    (void)retrieved_num;
    (void)final_val;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}