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
//<ID> 1548
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("fuzz_test");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    char *printed = cJSON_Print(root);

    // step 3: Operate & Validate
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *name_from_meta = cJSON_GetObjectItem(meta_item, "name");
    char *name_value = cJSON_GetStringValue(name_from_meta);
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    (void)name_value; (void)has_enabled;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}