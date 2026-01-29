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
//<ID> 1293
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
    cJSON *detached = NULL;
    cJSON *retrieved = NULL;
    cJSON *true_added = NULL;
    double pi_value = 0.0;
    double retrieved_value = 0.0;
    cJSON_bool add_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159);
    add_result = cJSON_AddItemToObject(root, "pi", num_item);
    true_added = cJSON_AddTrueToObject(root, "enabled");
    true_item = cJSON_CreateTrue();
    add_result = cJSON_AddItemToObject(root, "enabled2", true_item);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromObject(root, "pi");
    pi_value = cJSON_GetNumberValue(detached);
    cJSON_AddItemToObject(root, "pi_readded", detached);
    retrieved = cJSON_GetObjectItem(root, "pi_readded");
    retrieved_value = cJSON_GetNumberValue(retrieved);
    (void)pi_value;
    (void)retrieved_value;
    (void)true_added;
    (void)add_result;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}