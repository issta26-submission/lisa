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
//<ID> 1600
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
    cJSON *retrieved_copy = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON_bool added_num = 0;
    cJSON_bool added_child = 0;
    cJSON_bool added_dup = 0;
    cJSON_bool is_obj = 0;
    double extracted_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(2.71828);

    // step 3: Configure
    added_num = cJSON_AddItemToObjectCS(child, "e", num_item);
    dup_child = cJSON_Duplicate(child, 1);
    added_child = cJSON_AddItemToObjectCS(root, "child", child);
    added_dup = cJSON_AddItemToObjectCS(root, "child_copy", dup_child);

    // step 4: Operate
    retrieved_copy = cJSON_GetObjectItem(root, "child_copy");
    retrieved_num = cJSON_GetObjectItem(retrieved_copy, "e");
    extracted_value = cJSON_GetNumberValue(retrieved_num);
    is_obj = cJSON_IsObject(retrieved_copy);

    // step 5: Validate
    validation_score = (int)added_num
        + (int)added_child
        + (int)added_dup
        + (int)is_obj
        + (int)(retrieved_copy != (cJSON *)0)
        + (int)(retrieved_num != (cJSON *)0)
        + (int)(extracted_value == 2.71828);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)extracted_value;
    // API sequence test completed successfully
    return 66;
}