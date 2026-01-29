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
//<ID> 1517
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_false = (cJSON *)0;
    cJSON *child_name = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *name_value = (char *)0;
    cJSON_bool added_false = 0;
    cJSON_bool added_name = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_false = cJSON_CreateFalse();
    child_name = cJSON_CreateString("example");

    // step 3: Configure
    added_false = cJSON_AddItemToObject(root, "flag", child_false);
    added_name = cJSON_AddItemToObject(root, "name", child_name);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "flag");
    is_false = cJSON_IsFalse(retrieved);
    retrieved = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(retrieved);

    // step 5: Validate
    validation_score = (int)added_false + (int)added_name + (int)(retrieved != (cJSON *)0) + (int)is_false + (int)(name_value != (char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)name_value;
    (void)is_false;
    // API sequence test completed successfully
    return 66;
}