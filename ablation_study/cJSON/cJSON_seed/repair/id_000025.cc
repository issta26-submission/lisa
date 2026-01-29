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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *false_item_initial = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, false_item_initial);
    cJSON *false_item_replacement = cJSON_CreateFalse();
    cJSON *counter = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "count", counter);

    // step 2: Configure
    cJSON *first_in_flags = cJSON_GetArrayItem(flags, 0);
    cJSON_bool did_replace = cJSON_ReplaceItemViaPointer(flags, first_in_flags, false_item_replacement);
    cJSON_bool is_bool_now = cJSON_IsBool(cJSON_GetArrayItem(flags, 0));
    double new_count = (double)((int)did_replace + (int)is_bool_now);
    cJSON_SetNumberHelper(counter, new_count);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}