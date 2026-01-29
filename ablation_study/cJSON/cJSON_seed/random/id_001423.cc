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
//<ID> 1423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *true_node = (cJSON *)0;
    cJSON *true_ref = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_obj = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a true boolean, attach to root
    root = cJSON_CreateObject();
    true_node = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_node);

    // step 3: Configure - create an object reference to the true node and attach to root
    true_ref = cJSON_CreateObjectReference(true_node);
    cJSON_AddItemToObject(root, "flag_ref", true_ref);

    // step 4: Operate - check type, retrieve the reference item and print the whole tree
    is_obj = cJSON_IsObject(root);
    retrieved = cJSON_GetObjectItemCaseSensitive(root, "flag_ref");
    printed = cJSON_Print(root);

    // step 5: Validate - derive a simple validation score from results
    validation_score = (int)is_obj + (int)(retrieved != (cJSON *)0) + (int)(true_ref != (cJSON *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)retrieved;
    (void)true_node;
    (void)true_ref;
    return 66;
}