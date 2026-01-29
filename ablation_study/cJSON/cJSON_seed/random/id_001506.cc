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
//<ID> 1506
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_alpha = (cJSON *)0;
    cJSON *child_beta = (cJSON *)0;
    cJSON *retrieved_before = (cJSON *)0;
    cJSON *retrieved_after = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_alpha = 0;
    cJSON_bool added_beta = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_alpha = cJSON_CreateString("hello");
    child_beta = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_alpha = cJSON_AddItemToObjectCS(root, "alpha", child_alpha);
    added_beta = cJSON_AddItemToObjectCS(root, "beta", child_beta);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_before = cJSON_GetObjectItem(root, "alpha");
    cJSON_DeleteItemFromObject(root, "alpha");
    retrieved_after = cJSON_GetObjectItem(root, "alpha");

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)(retrieved_before != (cJSON *)0) + (int)(retrieved_after == (cJSON *)0) + (int)cJSON_IsNumber(cJSON_GetObjectItem(root, "beta")) + (int)added_alpha + (int)added_beta;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_before;
    (void)retrieved_after;
    // API sequence test completed successfully
    return 66;
}