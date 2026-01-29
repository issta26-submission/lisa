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
//<ID> 1429
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
    cJSON *inner_true = (cJSON *)0;
    cJSON *child_ref = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_obj = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, a child object and a true boolean inside the child
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    inner_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "enabled", inner_true);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Configure - create an object reference to the child and attach it to root
    child_ref = cJSON_CreateObjectReference((const cJSON *)child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 4: Operate - retrieve the child object case-sensitively and check its type, then print the tree
    picked = cJSON_GetObjectItemCaseSensitive(root, "child");
    is_obj = cJSON_IsObject((const cJSON *const)picked);
    printed = cJSON_Print((const cJSON *)root);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)is_obj + (int)(picked != (cJSON *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed buffer and delete the whole JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)inner_true;
    (void)child_ref;
    return 66;
}