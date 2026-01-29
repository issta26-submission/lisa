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
//<ID> 1115
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
    cJSON *fetched_flag = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_num = 0.0;
    cJSON_bool is_flag_true = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and boolean, add a number via helper
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    num_item = cJSON_AddNumberToObject(root, "count", 42.0);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate - fetch object members and extract values
    fetched_num = cJSON_GetObjectItem(root, "count");
    fetched_flag = cJSON_GetObjectItem(root, "flag");
    retrieved_num = cJSON_GetNumberValue(fetched_num);
    is_flag_true = cJSON_IsTrue(fetched_flag);

    // step 4: Validate - serialize briefly and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_flag != (cJSON *)0) + (int)(retrieved_num == 42.0) + (int)(is_flag_true);
    (void)validation_score;

    // step 5: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}