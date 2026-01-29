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
//<ID> 1288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_node_const = (cJSON *)0;
    cJSON *num_node_dyn = (cJSON *)0;
    cJSON *found_const = (cJSON *)0;
    cJSON *found_dyn = (cJSON *)0;
    cJSON *found_missing = (cJSON *)0;
    cJSON_bool root_is_obj = 0;
    cJSON_bool found_is_invalid = 0;
    double const_value = 0.0;
    double dyn_value = 0.0;
    int validation_score = 0;
    cJSON_bool add_result1 = 0;
    cJSON_bool add_result2 = 0;

    // step 2: Initialize - create a root object and two number nodes
    root = cJSON_CreateObject();
    num_node_const = cJSON_CreateNumber(42.5);
    num_node_dyn = cJSON_CreateNumber(-3.14);

    // step 3: Configure - attach number nodes to the root object
    add_result1 = cJSON_AddItemToObject(root, "const", num_node_const);
    add_result2 = cJSON_AddItemToObject(root, "dyn", num_node_dyn);

    // step 4: Operate - retrieve items and query their types and values
    found_const = cJSON_GetObjectItem(root, "const");
    found_dyn = cJSON_GetObjectItem(root, "dyn");
    found_missing = cJSON_GetObjectItem(root, "nonexistent");
    root_is_obj = cJSON_IsObject(root);
    found_is_invalid = cJSON_IsInvalid(found_const);
    const_value = cJSON_GetNumberValue(found_const);
    dyn_value = cJSON_GetNumberValue(found_dyn);

    // step 5: Validate - compute a simple validation score using results (no branching)
    validation_score = (int)root_is_obj + (int)found_is_invalid + (int)(const_value == 42.5) + (int)(dyn_value < 0.0) + (int)(found_const != (cJSON *)0) + (int)(found_dyn != (cJSON *)0) + (int)add_result1 + (int)add_result2 + (int)(found_missing == (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}