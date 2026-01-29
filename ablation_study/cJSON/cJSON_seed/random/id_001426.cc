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
//<ID> 1426
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_obj = (cJSON *)0;
    cJSON *true_node = (cJSON *)0;
    cJSON *child_ref = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;
    cJSON_bool is_obj = 0;

    // step 2: Initialize - create root object, a child object and a true boolean node
    root = cJSON_CreateObject();
    child_obj = cJSON_CreateObject();
    true_node = cJSON_CreateTrue();

    // step 3: Configure - attach the boolean to the child object, attach child to root and add a reference to the child
    cJSON_AddItemToObject(child_obj, "flag", true_node);
    cJSON_AddItemToObject(root, "child", child_obj);
    child_ref = cJSON_CreateObjectReference(child_obj);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 4: Operate - retrieve the child from the root and check its type, then print the whole tree
    retrieved = cJSON_GetObjectItemCaseSensitive(root, "child");
    is_obj = cJSON_IsObject(retrieved);
    printed = cJSON_Print(root);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)is_obj + (int)(printed != (char *)0) + (int)(retrieved != (cJSON *)0) + (int)(child_ref != (cJSON *)0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    (void)validation_score;
    (void)retrieved;
    (void)printed;
    // API sequence test completed successfully
    return 66;
}