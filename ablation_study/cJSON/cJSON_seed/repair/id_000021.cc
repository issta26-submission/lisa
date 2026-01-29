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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *initial_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, initial_false);

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(settings, "flag_count", num_item);
    cJSON *replacement_false = cJSON_CreateFalse();
    cJSON *first_item = cJSON_GetArrayItem(flags, 0);

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(flags, first_item, replacement_false);
    cJSON_bool is_bool = cJSON_IsBool(replacement_false);
    cJSON_SetNumberHelper(num_item, (double)is_bool);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}