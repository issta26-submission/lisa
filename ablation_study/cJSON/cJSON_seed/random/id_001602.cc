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
//<ID> 1602
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
    cJSON *retrieved_dup = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_dup_num = (cJSON *)0;
    cJSON_bool added_num = 0;
    cJSON_bool added_child_to_root = 0;
    cJSON_bool added_dup_to_root = 0;
    cJSON_bool is_obj_orig = 0;
    cJSON_bool is_obj_dup = 0;
    double value_orig = 0.0;
    double value_dup = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(2.718281828);

    // step 3: Configure
    added_num = cJSON_AddItemToObjectCS(child, "e", num_item);
    added_child_to_root = cJSON_AddItemToObjectCS(root, "child", child);
    dup_child = cJSON_Duplicate(child, 1);
    added_dup_to_root = cJSON_AddItemToObjectCS(root, "child_copy", dup_child);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_dup = cJSON_GetObjectItem(root, "child_copy");
    retrieved_num = cJSON_GetObjectItem(retrieved_child, "e");
    retrieved_dup_num = cJSON_GetObjectItem(retrieved_dup, "e");
    value_orig = cJSON_GetNumberValue(retrieved_num);
    value_dup = cJSON_GetNumberValue(retrieved_dup_num);
    is_obj_orig = cJSON_IsObject(retrieved_child);
    is_obj_dup = cJSON_IsObject(retrieved_dup);

    // step 5: Validate
    validation_score = (int)added_num
        + (int)added_child_to_root
        + (int)added_dup_to_root
        + (int)(retrieved_child != (cJSON *)0)
        + (int)(retrieved_dup != (cJSON *)0)
        + (int)(retrieved_num != (cJSON *)0)
        + (int)(retrieved_dup_num != (cJSON *)0)
        + (int)(is_obj_orig)
        + (int)(is_obj_dup)
        + (int)(value_orig == value_dup);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_num;
    (void)retrieved_dup_num;
    (void)value_orig;
    (void)value_dup;
    // API sequence test completed successfully
    return 66;
}