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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *new_name = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    char *json_text = (char *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON_bool replace_ok = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("original");
    new_name = cJSON_CreateString("replaced");

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate
    replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "name", new_name);

    // step 5: Validate
    flag_item = cJSON_AddFalseToObject(root, "is_replaced");
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}