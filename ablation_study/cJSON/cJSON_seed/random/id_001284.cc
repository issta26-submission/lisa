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
//<ID> 1284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool is_obj = 0;
    cJSON_bool is_invalid = 0;
    double value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a numeric item
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "answer", num);

    // step 3: Configure - retrieve the numeric item from the object
    retrieved = cJSON_GetObjectItem(root, "answer");

    // step 4: Operate - check type, validity and extract the numeric value
    is_obj = cJSON_IsObject(root);
    is_invalid = cJSON_IsInvalid(retrieved);
    value = cJSON_GetNumberValue(retrieved);

    // step 5: Validate - compute a simple validation score to ensure APIs interacted
    validation_score = (int)is_obj + (int)(!is_invalid) + (int)(value == 42.5);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}