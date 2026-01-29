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
//<ID> 846
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON *age_item = cJSON_CreateNumber(30.0);
    cJSON *details_obj = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "age", age_item);
    cJSON_AddItemToObject(root, "details", details_obj);
    cJSON *greeting_item = cJSON_AddStringToObject(root, "greeting", "hello_world");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}