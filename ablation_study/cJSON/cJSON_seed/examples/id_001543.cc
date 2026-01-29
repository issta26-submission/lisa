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
//<ID> 1543
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_item);

    // step 2: Configure
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate & Validate
    cJSON_bool has_active = cJSON_HasObjectItem(root, "active");
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_value = cJSON_GetStringValue(got_name);
    char *printed = cJSON_Print(root);
    (void)has_active; (void)name_value; (void)active_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}