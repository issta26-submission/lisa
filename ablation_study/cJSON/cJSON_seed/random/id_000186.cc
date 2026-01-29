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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *old_str = (cJSON *)0;
    cJSON *new_str = (cJSON *)0;
    cJSON *replace_num = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON_bool replaced = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    old_str = cJSON_CreateString("value_old");

    // step 3: Configure
    cJSON_AddItemToObject(root, "key", old_str);

    // step 4: Operate
    new_str = cJSON_CreateString("value_new");
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "key", new_str);
    replace_num = cJSON_CreateNumber((double)replaced);
    cJSON_AddItemToObject(root, "replace_ok", replace_num);
    flag_item = cJSON_AddFalseToObject(root, "replacement_flag");

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}