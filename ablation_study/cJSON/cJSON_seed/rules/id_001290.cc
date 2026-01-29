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
//<ID> 1290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *true_item = NULL;
    cJSON *auto_true = NULL;
    cJSON *fetched = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    double fetched_value = 0.0;
    double detached_value = 0.0;
    cJSON_bool add_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14159265);
    add_ok = cJSON_AddItemToObject(root, "pi", num_item);
    true_item = cJSON_CreateTrue();
    add_ok = cJSON_AddItemToObject(root, "manual_flag", true_item);
    auto_true = cJSON_AddTrueToObject(root, "auto_flag");

    // step 3: Operate / Validate
    fetched = cJSON_GetObjectItem(root, "pi");
    fetched_value = cJSON_GetNumberValue(fetched);
    printed = cJSON_PrintUnformatted(root);
    detached = cJSON_DetachItemFromObject(root, "pi");
    detached_value = cJSON_GetNumberValue(detached);
    (void)add_ok;
    (void)fetched_value;
    (void)detached_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}