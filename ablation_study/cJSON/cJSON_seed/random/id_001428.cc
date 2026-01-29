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
//<ID> 1428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *inner = (cJSON *)0;
    cJSON *flag_true = (cJSON *)0;
    cJSON *inner_ref = (cJSON *)0;
    cJSON *retrieved_inner = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_obj = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an inner object and a true boolean
    root = cJSON_CreateObject();
    inner = cJSON_CreateObject();
    flag_true = cJSON_CreateTrue();

    // step 3: Configure - attach the boolean to the inner object and attach inner to root
    cJSON_AddItemToObject(inner, "flag", flag_true);
    cJSON_AddItemToObject(root, "inner", inner);

    // step 4: Operate - create an object reference to the inner object, attach it, print the tree
    inner_ref = cJSON_CreateObjectReference(inner);
    cJSON_AddItemToObject(root, "inner_ref", inner_ref);
    printed = cJSON_Print(root);

    // step 5: Validate - retrieve the original inner object by name and check its type
    retrieved_inner = cJSON_GetObjectItemCaseSensitive(root, "inner");
    is_obj = cJSON_IsObject(retrieved_inner);
    validation_score = (int)is_obj + (int)(inner_ref != (cJSON *)0) + (int)(retrieved_inner != (cJSON *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)flag_true;
    return 66;
}