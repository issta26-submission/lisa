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
//<ID> 1292
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
    cJSON *true_direct = NULL;
    cJSON *detached_num = NULL;
    cJSON *retrieved_true = NULL;
    double detached_value = 0.0;
    cJSON_bool retrieved_is_true = 0;
    cJSON_bool direct_is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.45);
    cJSON_AddItemToObject(root, "number_key", num_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "true_via_item", true_item);
    true_direct = cJSON_AddTrueToObject(root, "true_direct");

    // step 3: Operate / Validate
    detached_num = cJSON_DetachItemFromObject(root, "number_key");
    detached_value = cJSON_GetNumberValue(detached_num);
    retrieved_true = cJSON_GetObjectItem(root, "true_via_item");
    retrieved_is_true = cJSON_IsTrue(retrieved_true);
    direct_is_true = cJSON_IsTrue(true_direct);
    (void)detached_value;
    (void)retrieved_is_true;
    (void)direct_is_true;

    // step 4: Cleanup
    cJSON_Delete(detached_num);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}