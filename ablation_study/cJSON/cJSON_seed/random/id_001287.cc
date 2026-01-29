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
//<ID> 1287
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
    cJSON_bool root_is_obj = 0;
    cJSON_bool item_invalid = 0;
    double extracted_value = 0.0;
    int validation_score = 0;
    memset(&validation_score, 0, sizeof validation_score);

    // step 2: Initialize - create an object root and a numeric node
    root = cJSON_CreateObject();
    num_node = cJSON_CreateNumber(123.456);

    // step 3: Configure - attach the number to the root and retrieve it
    cJSON_AddItemToObject(root, "num", num_node);
    retrieved = cJSON_GetObjectItem(root, "num");

    // step 4: Operate - inspect types and extract numeric value
    root_is_obj = cJSON_IsObject(root);
    item_invalid = cJSON_IsInvalid(retrieved);
    extracted_value = cJSON_GetNumberValue(retrieved);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)root_is_obj + (int)item_invalid + (int)extracted_value;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}