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
//<ID> 592
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3];
    nums[0] = 1.1;
    nums[1] = 2.2;
    nums[2] = 3.3;
    cJSON *values_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_bool add_values_ok = cJSON_AddItemToObject(root, "values", values_array);
    cJSON *answer_item = cJSON_CreateNumber(42.0);
    cJSON_bool add_answer_ok = cJSON_AddItemToObject(root, "answer", answer_item);

    // step 2: Configure
    cJSON *retrieved_answer = cJSON_GetObjectItem(root, "answer");
    double before_replace = cJSON_GetNumberValue(retrieved_answer);
    cJSON *replacement = cJSON_CreateNumber(84.0);

    // step 3: Operate
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(root, retrieved_answer, replacement);
    cJSON *new_answer = cJSON_GetObjectItem(root, "answer");
    double after_replace = cJSON_GetNumberValue(new_answer);
    cJSON *first_elem = cJSON_GetArrayItem(values_array, 0);
    double first_value = cJSON_GetNumberValue(first_elem);

    // step 4: Validate & Cleanup
    (void)add_values_ok;
    (void)add_answer_ok;
    (void)before_replace;
    (void)replaced_ok;
    (void)after_replace;
    (void)first_value;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}