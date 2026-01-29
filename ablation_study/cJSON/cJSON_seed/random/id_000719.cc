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
//<ID> 719
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
    cJSON *active_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_active = 0;
    cJSON_bool name_is_string = 0;
    cJSON_bool active_is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example");
    active_item = cJSON_CreateFalse();

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_active = cJSON_AddItemToObject(root, "active", active_item);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_is_string = cJSON_IsString(retrieved_name);
    active_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "active"));
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_name + (int)added_active + (int)name_is_string + (int)active_is_false + (printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}