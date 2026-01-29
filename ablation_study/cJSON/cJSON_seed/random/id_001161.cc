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
//<ID> 1161
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *flags_arr = (cJSON *)0;
    cJSON *bool_true = (cJSON *)0;
    cJSON *enabled_bool = (cJSON *)0;
    cJSON *fetched_flags = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    cJSON *fetched_enabled = (cJSON *)0;
    cJSON_bool is_elem_bool = 0;
    cJSON_bool is_enabled_bool = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    flags_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags_arr);

    // step 3: Configure - create boolean items and add them to array and object
    bool_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags_arr, bool_true);
    enabled_bool = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "enabled", enabled_bool);

    // step 4: Operate - retrieve the array, its first element, and the enabled member
    fetched_flags = cJSON_GetObjectItem(root, "flags");
    fetched_elem0 = cJSON_GetArrayItem(fetched_flags, 0);
    fetched_enabled = cJSON_GetObjectItem(root, "enabled");
    is_elem_bool = cJSON_IsBool(fetched_elem0);
    is_enabled_bool = cJSON_IsBool(fetched_enabled);

    // step 5: Validate - compute a simple validation score from retrievals and type checks
    validation_score = (int)(fetched_flags != (cJSON *)0) + (int)(fetched_elem0 != (cJSON *)0) + (int)(fetched_enabled != (cJSON *)0) + (int)is_elem_bool + (int)is_enabled_bool;
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}