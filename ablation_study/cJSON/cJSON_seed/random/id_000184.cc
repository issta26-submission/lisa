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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *field_initial = (cJSON *)0;
    cJSON *field_replacement = (cJSON *)0;
    cJSON *retrieved_field = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    char *json_text = (char *)0;
    cJSON_bool replace_ok = (cJSON_bool)0;
    char *retrieved_value = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    field_initial = cJSON_CreateString("initial_value");
    field_replacement = cJSON_CreateString("replaced_value");

    // step 3: Configure
    cJSON_AddItemToObject(root, "field", field_initial);
    flag_item = cJSON_AddFalseToObject(root, "active_flag");

    // step 4: Operate
    replace_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "field", field_replacement);

    // step 5: Validate
    retrieved_field = cJSON_GetObjectItem(root, "field");
    retrieved_value = cJSON_GetStringValue(retrieved_field);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}