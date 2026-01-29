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
//<ID> 1280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *first_num = (cJSON *)0;
    cJSON *second_num = (cJSON *)0;
    cJSON *retrieved_first = (cJSON *)0;
    cJSON *retrieved_second = (cJSON *)0;
    cJSON_bool root_is_object = 0;
    cJSON_bool first_is_invalid = 0;
    double first_value = 0.0;
    double second_value = 0.0;
    cJSON_bool add_result1 = 0;
    cJSON_bool add_result2 = 0;
    int validation_score = 0;

    // step 2: Initialize - create a root object and a numeric child
    root = cJSON_CreateObject();
    first_num = cJSON_CreateNumber(42.0);
    add_result1 = cJSON_AddItemToObject(root, "first", first_num);

    // step 3: Configure - retrieve the added item and inspect types/status
    retrieved_first = cJSON_GetObjectItem(root, "first");
    root_is_object = cJSON_IsObject(root);
    first_is_invalid = cJSON_IsInvalid(retrieved_first);
    first_value = cJSON_GetNumberValue(retrieved_first);

    // step 4: Operate - create a second number derived from the first and attach it
    second_num = cJSON_CreateNumber(first_value + 8.0);
    add_result2 = cJSON_AddItemToObject(root, "second", second_num);
    retrieved_second = cJSON_GetObjectItem(root, "second");
    second_value = cJSON_GetNumberValue(retrieved_second);

    // step 5: Validate - compute a simple validation metric using returned values and flags
    validation_score = (int)root_is_object + (int)(first_is_invalid == 0) + (int)add_result1 + (int)add_result2 + (int)first_value + (int)second_value;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}