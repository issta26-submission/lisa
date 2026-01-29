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
//<ID> 1503
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *item_a = (cJSON *)0;
    cJSON *item_b = (cJSON *)0;
    cJSON *retr_a_before = (cJSON *)0;
    cJSON *retr_b_before = (cJSON *)0;
    cJSON *retr_a_after = (cJSON *)0;
    cJSON *retr_b_after = (cJSON *)0;
    char *printed_before = (char *)0;
    char *printed_after = (char *)0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    item_a = cJSON_CreateString("value_alpha");
    item_b = cJSON_CreateNumber(123.0);

    // step 3: Configure
    added_a = cJSON_AddItemToObjectCS(root, "alpha", item_a);
    added_b = cJSON_AddItemToObjectCS(root, "beta", item_b);

    // step 4: Operate
    printed_before = cJSON_PrintUnformatted(root);
    retr_a_before = cJSON_GetObjectItem(root, "alpha");
    retr_b_before = cJSON_GetObjectItem(root, "beta");
    cJSON_DeleteItemFromObject(root, "beta");
    printed_after = cJSON_PrintUnformatted(root);
    retr_a_after = cJSON_GetObjectItem(root, "alpha");
    retr_b_after = cJSON_GetObjectItem(root, "beta");

    // step 5: Validate
    validation_score = (int)(printed_before != (char *)0) + (int)(printed_after != (char *)0) + (int)added_a + (int)added_b + (int)(retr_a_before != (cJSON *)0) + (int)(retr_b_before != (cJSON *)0) + (int)(retr_a_after != (cJSON *)0) + (int)(retr_b_after == (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed_before);
    cJSON_free((void *)printed_after);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retr_a_before;
    (void)retr_b_before;
    (void)retr_a_after;
    (void)retr_b_after;
    // API sequence test completed successfully
    return 66;
}