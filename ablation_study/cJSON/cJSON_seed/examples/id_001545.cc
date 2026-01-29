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
//<ID> 1545
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "example-name");
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddNumberToObject(root, "count", 5.0);

    // step 2: Configure
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double updated_count = cJSON_SetNumberHelper(count_item, 42.0);
    (void)updated_count;

    // step 3: Operate & Validate
    char *printed = cJSON_Print(root);
    const char *name_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    (void)name_value; (void)has_enabled; (void)count_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}