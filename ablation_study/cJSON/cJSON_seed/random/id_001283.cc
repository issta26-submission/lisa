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
//<ID> 1283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_node = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool is_obj = 0;
    cJSON_bool is_invalid = 0;
    double num_value = 0.0;
    cJSON_bool add_result = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a numeric node
    root = cJSON_CreateObject();
    num_node = cJSON_CreateNumber(42.5);

    // step 3: Configure - attach the numeric node to the root object
    add_result = cJSON_AddItemToObject(root, "answer", num_node);

    // step 4: Operate - retrieve the item, inspect types and values
    retrieved = cJSON_GetObjectItem(root, "answer");
    is_obj = cJSON_IsObject(root);
    is_invalid = cJSON_IsInvalid(retrieved);
    num_value = cJSON_GetNumberValue(retrieved);

    // step 5: Validate - derive a simple validation score based on checks and value
    validation_score = (int)is_obj + (int)(!is_invalid) + (int)(num_value == 42.5) + (int)add_result + (int)(retrieved != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - free all allocated cJSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}