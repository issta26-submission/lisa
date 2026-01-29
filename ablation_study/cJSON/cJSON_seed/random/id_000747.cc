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
//<ID> 747
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *flags = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *added_bool_item = (cJSON *)0;
    cJSON *retrieved_enabled = (cJSON *)0;
    cJSON *retrieved_flags = (cJSON *)0;
    cJSON_bool added_array_result = 0;
    cJSON_bool added_flags_to_root = 0;
    int flags_count = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    flags = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    added_array_result = cJSON_AddItemToArray(flags, false_item);
    added_flags_to_root = cJSON_AddItemToObject(root, "flags", flags);

    // step 4: Operate
    retrieved_enabled = cJSON_GetObjectItem(root, "enabled");
    retrieved_flags = cJSON_GetObjectItem(root, "flags");
    flags_count = cJSON_GetArraySize(retrieved_flags);

    // step 5: Validate
    validation_score = (retrieved_enabled != (cJSON *)0) + (flags_count == 1) + (added_array_result != 0) + (added_flags_to_root != 0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}