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
//<ID> 811
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
    cJSON *false_item = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    cJSON_bool added_flag = 0;
    cJSON_bool added_child = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool added_null = 0;
    cJSON_bool is_flag_null = 0;
    cJSON_bool is_null_null = 0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    null_item = cJSON_CreateNull();

    // step 3: Configure
    added_flag = cJSON_AddItemToObject(child, "flag", false_item);
    added_child = cJSON_AddItemToObject(root, "child", child);
    added_ref = cJSON_AddItemReferenceToObject(root, "refFlag", false_item);
    added_null = cJSON_AddItemToObject(root, "nullvalue", null_item);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_flag = cJSON_GetObjectItem(root, "refFlag");
    retrieved_null = cJSON_GetObjectItem(root, "nullvalue");
    is_flag_null = cJSON_IsNull(retrieved_flag);
    is_null_null = cJSON_IsNull(retrieved_null);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_child != (cJSON *)0) + (retrieved_flag != (cJSON *)0) + (retrieved_null != (cJSON *)0) + (int)is_flag_null + (int)is_null_null + (int)added_flag + (int)added_child + (int)added_ref + (int)added_null + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}