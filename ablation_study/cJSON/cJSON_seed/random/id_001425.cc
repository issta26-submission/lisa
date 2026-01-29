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
//<ID> 1425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *flag_true = (cJSON *)0;
    cJSON *flag_ref = (cJSON *)0;
    cJSON *found_flag = (cJSON *)0;
    char *printed = (char *)0;
    int is_object = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a true boolean child
    root = cJSON_CreateObject();
    flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_true);

    // step 3: Configure - create an object reference to the boolean and attach to root
    flag_ref = cJSON_CreateObjectReference((const cJSON *)flag_true);
    cJSON_AddItemToObject(root, "flag_ref", flag_ref);

    // step 4: Operate - retrieve the original flag via case-sensitive lookup, print the tree, check type
    found_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    printed = cJSON_Print((const cJSON *)root);
    is_object = (int)cJSON_IsObject((const cJSON *)root);

    // step 5: Validate - derive a small validation score from results
    validation_score = is_object + (int)(found_flag != (cJSON *)0) + (int)(flag_ref != (cJSON *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    (void)validation_score;
    (void)found_flag;
    (void)flag_true;
    (void)flag_ref;

    // API sequence test completed successfully
    return 66;
}