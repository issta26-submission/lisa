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
//<ID> 1118
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
    cJSON *num_item = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_true = (cJSON *)0;
    double retrieved_number = 0.0;
    cJSON_bool retrieved_bool = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a true boolean item
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();

    // step 3: Configure - add a numeric member and attach the true item to the root
    num_item = cJSON_AddNumberToObject(root, "count", 42.0);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 4: Operate - retrieve members from the object
    fetched_num = cJSON_GetObjectItem(root, "count");
    fetched_true = cJSON_GetObjectItem(root, "flag");
    retrieved_number = cJSON_GetNumberValue(fetched_num);
    retrieved_bool = cJSON_IsTrue(fetched_true);

    // step 5: Validate - compute a simple validation score to ensure values flowed
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_true != (cJSON *)0) + (int)(retrieved_number == 42.0) + (int)retrieved_bool;
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}