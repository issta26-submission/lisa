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
//<ID> 1296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *true_item = NULL;
    cJSON *true_shortcut = NULL;
    cJSON *detached_item = NULL;
    cJSON *retrieved_num = NULL;
    double pi_value = 0.0;
    double aggregate = 0.0;
    cJSON_bool added_num = 0;
    cJSON_bool added_true_manual = 0;
    cJSON_bool is_detached_true = 0;
    cJSON_bool is_shortcut_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159);
    added_num = cJSON_AddItemToObject(root, "pi", num_item);
    true_item = cJSON_CreateTrue();
    added_true_manual = cJSON_AddItemToObject(root, "manual_true", true_item);
    true_shortcut = cJSON_AddTrueToObject(root, "shortcut_true");

    // step 3: Operate / Validate
    retrieved_num = cJSON_GetObjectItem(root, "pi");
    pi_value = cJSON_GetNumberValue(retrieved_num);
    detached_item = cJSON_DetachItemFromObject(root, "manual_true");
    is_detached_true = cJSON_IsTrue(detached_item);
    is_shortcut_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "shortcut_true"));
    aggregate = pi_value + (double)is_detached_true + (double)is_shortcut_true;
    (void)added_num;
    (void)added_true_manual;
    (void)aggregate;

    // step 4: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}