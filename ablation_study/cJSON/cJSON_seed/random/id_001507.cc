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
//<ID> 1507
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_str = (cJSON *)0;
    cJSON *child_num = (cJSON *)0;
    cJSON *retrieved_str_before = (cJSON *)0;
    cJSON *retrieved_str_after = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_str = 0;
    cJSON_bool added_num = 0;
    double num_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_str = cJSON_CreateString("example");
    child_num = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_str = cJSON_AddItemToObjectCS(root, "key_str", child_str);
    added_num = cJSON_AddItemToObjectCS(root, "key_num", child_num);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_str_before = cJSON_GetObjectItem(root, "key_str");
    retrieved_num = cJSON_GetObjectItem(root, "key_num");
    num_value = cJSON_GetNumberValue(retrieved_num);
    cJSON_DeleteItemFromObject(root, "key_str");
    retrieved_str_after = cJSON_GetObjectItem(root, "key_str");

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_str + (int)added_num + (int)(retrieved_str_before != (cJSON *)0) + (int)(retrieved_str_after == (cJSON *)0) + (int)(num_value == 42.0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_num;
    (void)retrieved_str_before;
    (void)retrieved_str_after;
    // API sequence test completed successfully
    return 66;
}