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
//<ID> 1422
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
    cJSON *found_ref = (cJSON *)0;
    char *printed = (char *)0;
    int is_obj = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a true boolean
    root = cJSON_CreateObject();
    flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_true);

    // step 3: Configure - create an object reference to the boolean and add to root
    flag_ref = cJSON_CreateObjectReference(flag_true);
    cJSON_AddItemToObject(root, "flag_ref", flag_ref);

    // step 4: Operate - retrieve the reference item case-sensitively, check if it's an object, and print the tree
    found_ref = cJSON_GetObjectItemCaseSensitive(root, "flag_ref");
    is_obj = (int)cJSON_IsObject(found_ref);
    printed = cJSON_Print(root);

    // step 5: Validate - aggregate simple validation score from results
    validation_score = is_obj + (int)(found_ref != (cJSON *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed string and delete the root (which contains children and references)
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    (void)validation_score;
    (void)flag_true;
    (void)flag_ref;
    (void)found_ref;
    // API sequence test completed successfully
    return 66;
}