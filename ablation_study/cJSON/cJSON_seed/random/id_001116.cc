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
//<ID> 1116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *fetched_value = (cJSON *)0;
    cJSON *fetched_flag = (cJSON *)0;
    double retrieved_number = 0.0;
    cJSON_bool retrieved_flag_true = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a true boolean
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();

    // step 3: Configure - add a numeric member and attach the true boolean to the object
    cJSON_AddNumberToObject(root, "value", 123.0);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 4: Operate - fetch members from the object
    fetched_value = cJSON_GetObjectItem(root, "value");
    fetched_flag = cJSON_GetObjectItem(root, "flag");

    // step 5: Validate - extract typed values and compute a simple validation score
    retrieved_number = cJSON_GetNumberValue(fetched_value);
    retrieved_flag_true = cJSON_IsTrue(fetched_flag);
    validation_score = (int)(fetched_value != (cJSON *)0) + (int)(fetched_flag != (cJSON *)0) + (int)(retrieved_number == 123.0) + (int)(retrieved_flag_true);

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}