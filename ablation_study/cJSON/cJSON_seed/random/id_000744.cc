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
//<ID> 744
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *flags_array = NULL;
    cJSON *false_item = NULL;
    cJSON *added_enabled = NULL;
    cJSON *retrieved_flags = NULL;
    int flags_count = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    flags_array = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(flags_array, false_item);
    cJSON_AddItemToObject(root, "flags", flags_array);
    added_enabled = cJSON_AddBoolToObject(root, "enabled", 0);

    // step 4: Operate
    retrieved_flags = cJSON_GetObjectItem(root, "flags");
    flags_count = cJSON_GetArraySize(retrieved_flags);

    // step 5: Validate
    validation_score = (root != NULL) + (retrieved_flags != NULL) + (flags_count >= 0) + (added_enabled != NULL);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}