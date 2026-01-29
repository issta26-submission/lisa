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
//<ID> 1496
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *false_node = (cJSON *)0;
    cJSON *null_node = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;
    cJSON_bool added_false = 0;
    cJSON_bool added_null = 0;
    cJSON_bool is_null_flag = 0;

    // step 2: Initialize - create root and nodes
    root = cJSON_CreateObject();
    false_node = cJSON_CreateFalse();
    null_node = cJSON_CreateNull();

    // step 3: Configure - add items to root
    added_false = cJSON_AddItemToObject(root, "flag", false_node);
    added_null = cJSON_AddItemToObject(root, "nullable", null_node);

    // step 4: Operate - produce unformatted JSON and retrieve the null item
    printed = cJSON_PrintUnformatted(root);
    retrieved_null = cJSON_GetObjectItem(root, "nullable");
    is_null_flag = cJSON_IsNull(retrieved_null);

    // step 5: Validate - compute a small validation score from results
    validation_score = (int)(printed != (char *)0) + (int)added_false + (int)added_null + (int)is_null_flag;

    // step 6: Cleanup - free printed buffer and delete the JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_null;
    (void)false_node;
    (void)null_node;
    (void)added_false;
    (void)added_null;
    (void)is_null_flag;
    // API sequence test completed successfully
    return 66;
}