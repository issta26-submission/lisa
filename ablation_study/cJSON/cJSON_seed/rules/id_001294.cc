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
//<ID> 1294
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *pi_item = NULL;
    cJSON *e_item = NULL;
    cJSON *true_item = NULL;
    cJSON *true_added = NULL;
    cJSON *detached = NULL;
    cJSON *flag_b = NULL;
    char *printed = NULL;
    double pi_val = 0.0;
    double e_val = 0.0;
    double detached_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi_item);
    e_item = cJSON_CreateNumber(2.71828);
    cJSON_AddItemToObject(root, "e", e_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag_a", true_item);
    true_added = cJSON_AddTrueToObject(root, "flag_b");

    // step 3: Operate / Validate
    pi_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    e_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "e"));
    detached = cJSON_DetachItemFromObject(root, "e");
    detached_val = cJSON_GetNumberValue(detached);
    cJSON_AddItemToObject(root, "e_restored", detached);
    flag_b = cJSON_GetObjectItem(root, "flag_b");
    printed = cJSON_PrintUnformatted(root);
    (void)pi_val;
    (void)e_val;
    (void)detached_val;
    (void)flag_b;
    (void)true_added;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}