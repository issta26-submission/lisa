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
//<ID> 1410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers_array = cJSON_CreateArray();
    cJSON *shared_num = cJSON_CreateNumber(7.0);

    // step 2: Configure
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON *flag_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemReferenceToArray(numbers_array, shared_num);
    cJSON_AddItemToObject(root, "values", numbers_array);
    cJSON *pi_dup = cJSON_Duplicate(pi_item, 1);
    cJSON_AddItemToObject(root, "pi_copy", pi_dup);

    // step 3: Operate & Validate
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON *retrieved_flag = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_true = cJSON_IsTrue(retrieved_flag);
    cJSON_AddNumberToObject(root, "pi_incremented", pi_val + (double)is_true);
    cJSON *shared_dup = cJSON_Duplicate(shared_num, 0);
    cJSON_AddItemToArray(numbers_array, shared_dup);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(shared_num);
    // API sequence test completed successfully
    return 66;
}