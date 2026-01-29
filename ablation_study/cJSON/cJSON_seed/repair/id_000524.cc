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
//<ID> 524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "inner", inner);
    cJSON *name = cJSON_CreateString("unit_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(inner, "flag", flag_false);

    // step 2: Configure
    cJSON *replacement_name = cJSON_CreateString("replaced_name");
    cJSON_bool ok_replace = cJSON_ReplaceItemViaPointer(root, name, replacement_name);
    (void)ok_replace;

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON *copied_name = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(inner, "copied_name", copied_name);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}