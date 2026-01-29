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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *orig = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON_bool replace_ok = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("initial value");

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "key", orig);

    // step 4: Operate
    replacement = cJSON_CreateString("replaced value");
    replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "key", replacement);
    flag_false = cJSON_AddFalseToObject(root, "enabled");

    // step 5: Validate
    cJSON_AddBoolToObject(root, "add_ok", add_ok);
    cJSON_AddBoolToObject(root, "replace_ok", replace_ok);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}