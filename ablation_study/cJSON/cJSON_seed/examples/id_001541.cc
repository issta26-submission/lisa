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
//<ID> 1541
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "example_name");
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    cJSON *retrieved_name_item = cJSON_GetObjectItem(root, "name");
    char *name_value = cJSON_GetStringValue(retrieved_name_item);
    cJSON *name_copy_item = cJSON_AddStringToObject(root, "name_copy", name_value);

    // step 3: Operate & Validate
    cJSON_bool has_name_copy = cJSON_HasObjectItem(root, "name_copy");
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)name_item; (void)active_item; (void)retrieved_name_item; (void)name_copy_item; (void)has_name_copy;
    return 66;
}