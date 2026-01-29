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
//<ID> 1286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool add_result = 0;
    cJSON_bool root_is_obj = 0;
    cJSON_bool retrieved_is_invalid = 0;
    double number_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create a root object and a numeric item
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.5);

    // step 3: Configure - add the numeric item to the root object
    add_result = cJSON_AddItemToObject(root, "answer", num_item);

    // step 4: Operate - retrieve the item and inspect its numeric value and types
    retrieved = cJSON_GetObjectItem(root, "answer");
    number_value = cJSON_GetNumberValue(retrieved);
    root_is_obj = cJSON_IsObject(root);
    retrieved_is_invalid = cJSON_IsInvalid(retrieved);

    // step 5: Validate - combine results into a small validation score (no branches)
    validation_score = (int)add_result + (int)(retrieved != (cJSON *)0) + (int)(number_value == 42.5) + (int)root_is_obj + (int)retrieved_is_invalid;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}