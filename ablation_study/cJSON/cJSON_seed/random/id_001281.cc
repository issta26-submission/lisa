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
//<ID> 1281
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
    cJSON *next_item = (cJSON *)0;
    cJSON *first_num = (cJSON *)0;
    cJSON *second_num = (cJSON *)0;
    double val1 = 0.0;
    double val2 = 0.0;
    cJSON_bool root_is_obj = 0;
    cJSON_bool num_invalid = 0;
    cJSON_bool add_result = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a numeric node
    root = cJSON_CreateObject();
    first_num = cJSON_CreateNumber(42.0);
    add_result = cJSON_AddItemToObject(root, "answer", first_num);

    // step 3: Configure - retrieve the numeric node and inspect types/validity
    num_item = cJSON_GetObjectItem(root, "answer");
    val1 = cJSON_GetNumberValue(num_item);
    root_is_obj = cJSON_IsObject(root);
    num_invalid = cJSON_IsInvalid(num_item);

    // step 4: Operate - create a derived number and attach it, then read it back
    second_num = cJSON_CreateNumber(val1 + 8.0);
    cJSON_AddItemToObject(root, "next", second_num);
    next_item = cJSON_GetObjectItem(root, "next");
    val2 = cJSON_GetNumberValue(next_item);

    // step 5: Validate - compute a simple validation score from API results
    validation_score = (int)root_is_obj + (int)add_result + (int)(val1 == 42.0) + (int)(val2 == 50.0) + (int)num_invalid;
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}