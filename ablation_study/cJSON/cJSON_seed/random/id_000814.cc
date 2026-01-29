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
//<ID> 814
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
    cJSON *ref_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_alias = (cJSON *)0;
    cJSON *retrieved_ref_item = (cJSON *)0;
    cJSON_bool added_ref_to_child = 0;
    cJSON_bool added_flag_to_child = 0;
    cJSON_bool added_child_to_root = 0;
    cJSON_bool added_reference_in_root = 0;
    cJSON_bool is_missing_null = 0;
    cJSON_bool is_flag_false = 0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_obj = cJSON_CreateObject();
    ref_item = cJSON_CreateString("referenced_value");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_ref_to_child = cJSON_AddItemToObject(child_obj, "ref", ref_item);
    added_flag_to_child = cJSON_AddItemToObject(child_obj, "flag", false_item);
    added_child_to_root = cJSON_AddItemToObject(root, "child", child_obj);
    added_reference_in_root = cJSON_AddItemReferenceToObject(root, "alias", child_obj);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_alias = cJSON_GetObjectItem(root, "alias");
    retrieved_ref_item = cJSON_GetObjectItem(retrieved_alias, "ref");
    is_missing_null = cJSON_IsNull(cJSON_GetObjectItem(retrieved_child, "missing"));
    is_flag_false = cJSON_IsFalse(cJSON_GetObjectItem(retrieved_child, "flag"));

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_child != (cJSON *)0) + (retrieved_alias != (cJSON *)0) + (retrieved_ref_item != (cJSON *)0) + (int)is_missing_null + (int)is_flag_false + (int)added_ref_to_child + (int)added_flag_to_child + (int)added_child_to_root + (int)added_reference_in_root + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}