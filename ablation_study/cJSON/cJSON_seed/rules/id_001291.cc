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
//<ID> 1291
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
    cJSON *true_manual = NULL;
    cJSON *true_direct = NULL;
    cJSON *detached = NULL;
    cJSON *found = NULL;
    double num_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num_item);
    true_manual = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled_manual", true_manual);
    true_direct = cJSON_AddTrueToObject(root, "enabled_direct");

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromObject(root, "pi");
    num_val = cJSON_GetNumberValue(detached);
    cJSON_AddItemToObject(root, "pi_copy", detached);
    found = cJSON_GetObjectItem(root, "enabled_direct");
    (void)num_val;
    (void)found;
    (void)true_direct;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}