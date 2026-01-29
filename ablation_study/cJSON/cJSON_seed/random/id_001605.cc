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
//<ID> 1605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *dup_child = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    double retrieved_value = 0.0;
    cJSON_bool added_num = 0;
    cJSON_bool added_child = 0;
    cJSON_bool root_is_obj = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.456);

    // step 3: Configure
    added_num = cJSON_AddItemToObjectCS(child, "pi_like", num_item);
    added_child = cJSON_AddItemToObjectCS(root, "child", child);
    dup_child = cJSON_Duplicate(child, 1);

    // step 4: Operate
    root_is_obj = cJSON_IsObject(root);
    retrieved_num = cJSON_GetObjectItem(dup_child, "pi_like");
    retrieved_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    validation_score = (int)added_num
        + (int)added_child
        + (int)(dup_child != (cJSON *)0)
        + (int)root_is_obj
        + (int)(retrieved_num != (cJSON *)0)
        + (int)(retrieved_value == 123.456);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_child);
    (void)validation_score;
    (void)retrieved_value;
    (void)retrieved_num;
    // API sequence test completed successfully
    return 66;
}