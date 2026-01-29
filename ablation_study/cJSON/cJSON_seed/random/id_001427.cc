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
//<ID> 1427
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
    cJSON *flag_true = (cJSON *)0;
    cJSON *child_ref = (cJSON *)0;
    cJSON *found_flag = (cJSON *)0;
    cJSON *found_child_ref = (cJSON *)0;
    char *printed = (char *)0;
    int is_obj = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, a child object and a true boolean
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    flag_true = cJSON_CreateTrue();

    // step 3: Configure - populate child and attach members to root, including an object reference
    cJSON_AddStringToObject(child, "inner", "value");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "flag", flag_true);
    child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 4: Operate - retrieve items case-sensitively and print the whole tree
    found_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    found_child_ref = cJSON_GetObjectItemCaseSensitive(root, "child_ref");
    printed = cJSON_Print(root);

    // step 5: Validate - derive a small validation score using object check and presence
    is_obj = (int)cJSON_IsObject(found_child_ref);
    validation_score = is_obj + (int)(found_flag != (cJSON *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    (void)validation_score;
    (void)is_obj;
    (void)found_flag;
    (void)found_child_ref;
    // API sequence test completed successfully
    return 66;
}