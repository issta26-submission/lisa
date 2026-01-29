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
//<ID> 1285
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
    cJSON_bool obj_check = 0;
    cJSON_bool invalid_check = 0;
    double extracted_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a numeric node
    root = cJSON_CreateObject();
    num_node = cJSON_CreateNumber(42.5);

    // step 3: Configure - attach the numeric node to the root under the key "answer"
    cJSON_AddItemToObject(root, "answer", num_node);

    // step 4: Operate - retrieve the item and query its type and numeric value
    retrieved = cJSON_GetObjectItem(root, "answer");
    obj_check = cJSON_IsObject(retrieved);
    extracted_value = cJSON_GetNumberValue(retrieved);
    invalid_check = cJSON_IsInvalid(retrieved);

    // step 5: Validate - combine results into a simple validation score to ensure values flowed through
    validation_score = (int)(retrieved != (cJSON *)0) + (int)obj_check + (int)invalid_check + (int)extracted_value;
    (void)validation_score;

    // step 6: Cleanup - free the root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}