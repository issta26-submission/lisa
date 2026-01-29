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
//<ID> 1609
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
    cJSON *pi_item = (cJSON *)0;
    cJSON_bool added_num_to_child = 0;
    cJSON_bool added_child_to_root = 0;
    cJSON_bool added_dup_to_root = 0;
    cJSON_bool is_obj = 0;
    double num_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    added_num_to_child = cJSON_AddItemToObjectCS(child, "pi", cJSON_CreateNumber(123.456));
    added_child_to_root = cJSON_AddItemToObjectCS(root, "child", child);
    dup_child = cJSON_Duplicate(child, 1);
    added_dup_to_root = cJSON_AddItemToObjectCS(root, "child_copy", dup_child);

    // step 4: Operate
    is_obj = cJSON_IsObject(dup_child);
    pi_item = cJSON_GetObjectItem(dup_child, "pi");
    num_val = cJSON_GetNumberValue(pi_item);

    // step 5: Validate
    validation_score = (int)added_num_to_child
        + (int)added_child_to_root
        + (int)added_dup_to_root
        + (int)is_obj
        + (int)(pi_item != (cJSON *)0)
        + (int)(num_val > 100.0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)num_val;
    (void)pi_item;
    (void)dup_child;
    (void)child;
    (void)added_num_to_child;
    (void)added_child_to_root;
    (void)added_dup_to_root;
    (void)is_obj;
    // API sequence test completed successfully
    return 66;
}