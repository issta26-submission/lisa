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
//<ID> 1603
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
    double num_val = 0.0;
    double dup_num_val = 0.0;
    cJSON_bool added_orig = 0;
    cJSON_bool added_dup = 0;
    cJSON_bool is_obj = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    cJSON_AddItemToObjectCS(child, "alpha", num_item);
    added_orig = cJSON_AddItemToObjectCS(root, "child", child);
    dup_child = cJSON_Duplicate(child, 1);
    added_dup = cJSON_AddItemToObjectCS(root, "child_copy", dup_child);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_dup = cJSON_GetObjectItem(root, "child_copy");
    is_obj = cJSON_IsObject(retrieved_child);
    retrieved_num = cJSON_GetObjectItem(retrieved_child, "alpha");
    num_val = cJSON_GetNumberValue(retrieved_num);
    retrieved_num = cJSON_GetObjectItem(retrieved_dup, "alpha");
    dup_num_val = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    validation_score = (int)added_orig
        + (int)added_dup
        + (int)is_obj
        + (int)(num_val != 0.0)
        + (int)(dup_num_val != 0.0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)num_val;
    (void)dup_num_val;
    (void)retrieved_num;
    (void)retrieved_child;
    (void)retrieved_dup;
    // API sequence test completed successfully
    return 66;
}