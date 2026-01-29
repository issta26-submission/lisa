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
//<ID> 427
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *initial_num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", initial_num);
    cJSON *flag = cJSON_AddTrueToObject(root, "is_valid");

    // step 2: Configure
    cJSON *replacement_num = cJSON_CreateNumber(6.2830);
    double flag_num_val = (double)cJSON_IsTrue(flag);
    cJSON *flag_num_item = cJSON_CreateNumber(flag_num_val);
    cJSON_AddItemToObject(root, "valid_numeric", flag_num_item);

    // step 3: Operate
    cJSON_ReplaceItemInObject(root, "pi", replacement_num);
    cJSON *pi_item = cJSON_GetObjectItemCaseSensitive(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON *scaled_pi = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(root, "scaled_pi", scaled_pi);

    // step 4: Validate and Cleanup
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}