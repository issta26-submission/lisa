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
//<ID> 1604
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
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON_bool added_num_to_child = 0;
    cJSON_bool added_child_to_root = 0;
    cJSON_bool added_dup_to_root = 0;
    double num_value = 0.0;
    cJSON_bool is_obj_root = 0;
    cJSON_bool is_obj_dup = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    added_num_to_child = cJSON_AddItemToObjectCS(child, "pi", num_item);
    dup_child = cJSON_Duplicate(child, 1);
    added_child_to_root = cJSON_AddItemToObjectCS(root, "child", child);
    added_dup_to_root = cJSON_AddItemToObjectCS(root, "child_copy", dup_child);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_num = cJSON_GetObjectItem(retrieved_child, "pi");
    num_value = cJSON_GetNumberValue(retrieved_num);
    is_obj_root = cJSON_IsObject(root);
    is_obj_dup = cJSON_IsObject(dup_child);

    // step 5: Validate
    validation_score = (int)added_num_to_child
        + (int)added_child_to_root
        + (int)added_dup_to_root
        + (int)(is_obj_root)
        + (int)(is_obj_dup)
        + (int)(num_value > 3.14);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_num;
    (void)retrieved_child;
    (void)num_value;
    (void)is_obj_root;
    (void)is_obj_dup;
    // API sequence test completed successfully
    return 66;
}