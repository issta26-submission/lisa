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
//<ID> 859
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON_bool is_number_flag = 0;
    cJSON_bool added_parsed = 0;
    cJSON_bool added_detached = 0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse("[10, 20, 30]");

    // step 3: Operate on parsed array (detach an element and test its type)
    detached = cJSON_DetachItemFromArray(parsed, 1);
    is_number_flag = cJSON_IsNumber(detached);

    // step 4: Configure object and add items
    root = cJSON_CreateObject();
    added_parsed = cJSON_AddItemToObject(root, "numbers", parsed);
    added_detached = cJSON_AddItemToObject(root, "extracted", detached);

    // step 5: Validate results (aggregate simple checks)
    validation_score = (int)is_number_flag + (int)added_parsed + (int)added_detached + (detached != (cJSON *)0) + (root != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}