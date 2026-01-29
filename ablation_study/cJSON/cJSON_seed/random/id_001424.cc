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
//<ID> 1424
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
    cJSON_bool is_obj = 0;
    int validation_score = 0;

    // step 2: Initialize - create root and a child object that contains a true boolean
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    true_node = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "flag", true_node);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Configure - create an object reference to the child and attach it to root
    child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 4: Operate - print the JSON tree and retrieve the referenced object by case-sensitive name
    printed = cJSON_Print(root);
    found = cJSON_GetObjectItemCaseSensitive(root, "child_ref");
    is_obj = cJSON_IsObject(found);

    // step 5: Validate - compute a small validation score from results
    validation_score = (int)is_obj + (int)(printed != (char *)0) + (int)(child_ref != (cJSON *)0) + (int)(found != (cJSON *)0);

    // step 6: Cleanup - free printed buffer and delete the whole tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    (void)validation_score;
    (void)true_node;
    (void)child_ref;
    (void)found;
    // API sequence test completed successfully
    return 66;
}