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
//<ID> 370
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional);
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON_bool was_null = cJSON_IsNull(optional);
    cJSON *null_indicator = cJSON_CreateNumber((double)was_null);
    cJSON_AddItemToObject(root, "optional_was_null", null_indicator);

    // step 3: Operate and Validate
    cJSON *enabled = cJSON_GetObjectItem(root, "enabled");
    char *enabled_str = cJSON_GetStringValue(enabled); /* will be NULL for non-string, demonstrates API usage */
    (void)enabled_str; /* keep data flow explicit though value is unused */

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}