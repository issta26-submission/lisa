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
//<ID> 1502
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *item_alpha = (cJSON *)0;
    cJSON *item_beta = (cJSON *)0;
    cJSON *retrieved_alpha = (cJSON *)0;
    cJSON *retrieved_beta = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_alpha = 0;
    cJSON_bool added_beta = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    item_alpha = cJSON_CreateString("value_alpha");
    item_beta = cJSON_CreateNumber(123.0);

    // step 3: Configure
    added_alpha = cJSON_AddItemToObjectCS(root, "alpha", item_alpha);
    added_beta = cJSON_AddItemToObjectCS(root, "beta", item_beta);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_alpha = cJSON_GetObjectItem(root, "alpha");
    retrieved_beta = cJSON_GetObjectItem(root, "beta");

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_alpha + (int)added_beta + (int)cJSON_IsString(retrieved_alpha) + (int)cJSON_IsNumber(retrieved_beta);
    cJSON_DeleteItemFromObject(root, "alpha");
    retrieved_alpha = cJSON_GetObjectItem(root, "alpha");
    validation_score += (int)(retrieved_alpha == (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_beta;
    (void)added_alpha;
    (void)added_beta;
    // API sequence test completed successfully
    return 66;
}