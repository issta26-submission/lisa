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
//<ID> 1511
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_false = (cJSON *)0;
    cJSON *child_num = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_flag = 0;
    cJSON_bool added_num = 0;
    cJSON_bool is_false = 0;
    double num_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_false = cJSON_CreateFalse();
    child_num = cJSON_CreateNumber(3.14);

    // step 3: Configure
    added_flag = cJSON_AddItemToObject(root, "flag", child_false);
    added_num = cJSON_AddItemToObject(root, "pi", child_num);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_false = cJSON_GetObjectItem(root, "flag");
    is_false = cJSON_IsFalse(retrieved_false);
    retrieved_num = cJSON_GetObjectItem(root, "pi");
    num_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_flag + (int)added_num + (int)is_false + (int)(num_value == 3.14);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_num;
    (void)retrieved_false;
    (void)is_false;
    // API sequence test completed successfully
    return 66;
}