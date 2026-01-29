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
//<ID> 1601
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_obj = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *got_child = (cJSON *)0;
    cJSON *got_num = (cJSON *)0;
    cJSON *dup_child = (cJSON *)0;
    cJSON *dup_num = (cJSON *)0;
    cJSON_bool added_num = 0;
    cJSON_bool added_child = 0;
    cJSON_bool is_obj_root = 0;
    cJSON_bool is_obj_dup = 0;
    double value = 0.0;
    double dup_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_obj = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    added_num = cJSON_AddItemToObjectCS(child_obj, "pi", num_item);
    added_child = cJSON_AddItemToObjectCS(root, "child", child_obj);
    dup_root = cJSON_Duplicate(root, 1);

    // step 4: Operate
    got_child = cJSON_GetObjectItem(root, "child");
    got_num = cJSON_GetObjectItem(got_child, "pi");
    value = cJSON_GetNumberValue(got_num);
    dup_child = cJSON_GetObjectItem(dup_root, "child");
    dup_num = cJSON_GetObjectItem(dup_child, "pi");
    dup_value = cJSON_GetNumberValue(dup_num);
    is_obj_root = cJSON_IsObject(root);
    is_obj_dup = cJSON_IsObject(dup_root);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0)
        + (int)(child_obj != (cJSON *)0)
        + (int)(num_item != (cJSON *)0)
        + (int)added_num
        + (int)added_child
        + (int)(dup_root != (cJSON *)0)
        + (int)is_obj_root
        + (int)is_obj_dup
        + (int)(got_num != (cJSON *)0)
        + (int)(dup_num != (cJSON *)0)
        + (int)(value == dup_value);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    (void)got_child;
    (void)got_num;
    (void)dup_child;
    (void)dup_num;
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}