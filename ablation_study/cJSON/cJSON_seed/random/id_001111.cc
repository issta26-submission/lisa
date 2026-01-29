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
//<ID> 1111
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
    char *printed = (char *)0;
    double retrieved_num = 0.0;
    cJSON_bool is_true = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a true boolean
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();

    // step 3: Configure - add a numeric member and the true boolean to the root
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 4: Operate - retrieve the members from the object
    fetched_num = cJSON_GetObjectItem(root, "answer");
    fetched_true = cJSON_GetObjectItem(root, "flag");
    retrieved_num = cJSON_GetNumberValue(fetched_num);
    is_true = cJSON_IsTrue(fetched_true);

    // step 5: Validate - serialize for inspection and compute a small score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_true != (cJSON *)0) + (int)(retrieved_num == 42.0) + (int)(is_true);

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}