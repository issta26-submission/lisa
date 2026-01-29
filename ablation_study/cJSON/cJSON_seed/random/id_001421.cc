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
//<ID> 1421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *true_node = (cJSON *)0;
    cJSON *child_ref = (cJSON *)0;
    cJSON *found = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool found_is_object = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    true_node = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToObject(child, "status", true_node);
    cJSON_AddItemToObject(root, "child", child);
    child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 4: Operate
    found = cJSON_GetObjectItemCaseSensitive(root, "child");
    printed = cJSON_Print(root);

    // step 5: Validate
    found_is_object = cJSON_IsObject(found);
    validation_score = (int)found_is_object + (int)(child_ref != (cJSON *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)found;
    return 66;
}