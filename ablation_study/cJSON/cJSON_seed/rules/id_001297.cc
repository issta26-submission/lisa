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
//<ID> 1297
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
    cJSON *true_item_created = NULL;
    cJSON *true_item_shorthand = NULL;
    cJSON *detached_item = NULL;
    cJSON *tmp_item = NULL;
    char *printed = NULL;
    double pi_value = 0.0;
    cJSON_bool shorthand_is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num_item);
    true_item_created = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "explicit_true", true_item_created);
    true_item_shorthand = cJSON_AddTrueToObject(root, "shorthand_true");

    // step 3: Operate / Validate
    tmp_item = cJSON_GetObjectItem(root, "pi");
    pi_value = cJSON_GetNumberValue(tmp_item);
    tmp_item = cJSON_GetObjectItem(root, "shorthand_true");
    shorthand_is_true = cJSON_IsTrue(tmp_item);
    detached_item = cJSON_DetachItemFromObject(root, "explicit_true");
    printed = cJSON_PrintUnformatted(root);
    (void)pi_value;
    (void)shorthand_is_true;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}