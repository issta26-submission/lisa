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
//<ID> 815
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    cJSON_bool added_ref_null = 0;
    cJSON_bool added_ref_false = 0;
    cJSON_bool is_null = 0;
    cJSON_bool is_false = 0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    null_item = cJSON_CreateNull();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_ref_null = cJSON_AddItemReferenceToObject(root, "null_value", null_item);
    added_ref_false = cJSON_AddItemReferenceToObject(root, "false_value", false_item);

    // step 4: Operate
    retrieved_null = cJSON_GetObjectItem(root, "null_value");
    retrieved_false = cJSON_GetObjectItem(root, "false_value");
    is_null = cJSON_IsNull(retrieved_null);
    is_false = cJSON_IsFalse(retrieved_false);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (retrieved_null != (cJSON *)0) + (retrieved_false != (cJSON *)0) + (int)is_null + (int)is_false + (int)added_ref_null + (int)added_ref_false + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(null_item);
    cJSON_Delete(false_item);

    // API sequence test completed successfully
    return 66;
}