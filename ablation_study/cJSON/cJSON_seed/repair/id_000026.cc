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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *sub = cJSON_AddObjectToObject(config, "sub");
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *str_item = cJSON_CreateString("initial");

    // step 2: Configure
    cJSON_AddItemToArray(flags, false_item);
    cJSON_AddItemToArray(flags, str_item);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(sub, "answer", num_item);

    // step 3: Operate and Validate
    cJSON *updated_str = cJSON_CreateString("updated");
    cJSON_bool replace_res = cJSON_ReplaceItemViaPointer(flags, str_item, updated_str);
    cJSON_bool is_false_bool = cJSON_IsBool(false_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}