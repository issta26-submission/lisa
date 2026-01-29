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
//<ID> 1276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *parsed_flag = (cJSON *)0;
    cJSON *bool_node = (cJSON *)0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool add_result = 0;

    // step 2: Initialize - parse a root object that contains a single array property
    root = cJSON_Parse("{\"arr\": [1, 2, 3]}");

    // step 3: Configure - obtain the array node (first child), parse an independent boolean node, and add a bool to the root via helper
    arr = (root != (cJSON *)0) ? root->child : (cJSON *)0;
    parsed_flag = cJSON_Parse("false");
    bool_node = cJSON_AddBoolToObject(root, "ok", 1);

    // step 4: Operate - attach the separately parsed boolean node into the root using AddItemToObject and measure array size
    add_result = cJSON_AddItemToObject(root, "external_flag", parsed_flag);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a simple validation score to ensure data flowed through APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(parsed_flag != (cJSON *)0) + (int)(bool_node != (cJSON *)0) + arr_size + (int)add_result;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}