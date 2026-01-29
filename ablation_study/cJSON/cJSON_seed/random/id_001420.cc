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
//<ID> 1420
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
    cJSON *fetched = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;
    cJSON_bool is_obj = 0;

    // step 2: Initialize - create root object, a child object and a true boolean
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    true_node = cJSON_CreateTrue();

    // step 3: Configure - attach true to child and child to root
    cJSON_AddItemToObject(child, "enabled", true_node);
    cJSON_AddItemToObject(root, "config", child);

    // step 4: Operate - create an object reference to the child, attach it, fetch it and print root
    child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "config_ref", child_ref);
    fetched = cJSON_GetObjectItemCaseSensitive(root, "config_ref");
    is_obj = cJSON_IsObject(fetched);
    printed = cJSON_Print(root);

    // step 5: Validate - derive a small validation score from results
    validation_score = (int)is_obj + (int)(printed != (char *)0) + (int)(child_ref != (cJSON *)0) + (int)(true_node != (cJSON *)0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)fetched;
    (void)true_node;
    return 66;
}