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
//<ID> 1282
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
    cJSON *new_num = (cJSON *)0;
    cJSON_bool add_result = 0;
    cJSON_bool is_obj = 0;
    cJSON_bool is_invalid = 0;
    double number_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a numeric child
    root = cJSON_CreateObject();
    num_node = cJSON_CreateNumber(42.5);
    add_result = cJSON_AddItemToObject(root, "value", num_node);

    // step 3: Configure - retrieve the item and inspect types/validity
    retrieved = cJSON_GetObjectItem(root, "value");
    is_obj = cJSON_IsObject(root);
    is_invalid = cJSON_IsInvalid(retrieved);
    number_value = cJSON_GetNumberValue(retrieved);

    // step 4: Operate - create a new number based on the retrieved value and replace the original
    new_num = cJSON_CreateNumber(number_value * 2.0);
    cJSON_ReplaceItemInObject(root, "value", new_num);
    retrieved = cJSON_GetObjectItem(root, "value");
    number_value = cJSON_GetNumberValue(retrieved);

    // step 5: Validate - compute a simple score based on API results
    validation_score = (int)add_result + (int)is_obj + (int)(!is_invalid) + (int)(number_value == 85.0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}