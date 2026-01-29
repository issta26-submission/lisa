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
//<ID> 1289
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
    double read_value = 0.0;
    cJSON_bool root_is_object = (cJSON_bool)0;
    cJSON_bool retrieved_is_invalid = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create a root object and a numeric node
    root = cJSON_CreateObject();
    num_node = cJSON_CreateNumber(42.5);

    // step 3: Configure - attach the numeric node to the root under the key "value"
    cJSON_AddItemToObject(root, "value", num_node);

    // step 4: Operate - retrieve the item, inspect types and extract the numeric value
    retrieved = cJSON_GetObjectItem(root, "value");
    read_value = cJSON_GetNumberValue(retrieved);
    root_is_object = cJSON_IsObject(root);
    retrieved_is_invalid = cJSON_IsInvalid(retrieved);

    // step 5: Validate - compute a simple validation score from the results
    validation_score = (int)(retrieved != (cJSON *)0) + (int)root_is_object + (int)(!retrieved_is_invalid) + (int)(read_value == 42.5);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}