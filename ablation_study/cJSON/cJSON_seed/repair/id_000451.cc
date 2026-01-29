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
//<ID> 451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *status = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "status", status);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *new_status = cJSON_CreateString("updated");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, status, new_status);
    (void)replaced;

    // step 3: Operate & Validate
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    const char *status_value = cJSON_GetStringValue(status_item);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_was_false = cJSON_IsFalse(flag_item);
    (void)status_value;
    (void)flag_was_false;

    // step 4: Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}