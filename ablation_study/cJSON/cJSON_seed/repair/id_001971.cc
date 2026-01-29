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
//<ID> 1971
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "field", orig);
    cJSON *ref = cJSON_CreateStringReference("reference_value");
    cJSON_AddItemToObject(root, "alias", ref);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON *target = cJSON_GetObjectItem(root, "field");
    cJSON_ReplaceItemViaPointer(root, target, replacement);

    // step 3: Operate and Validate
    cJSON *got_field = cJSON_GetObjectItem(root, "field");
    const char *field_str = cJSON_GetStringValue(got_field);
    cJSON *got_alias = cJSON_GetObjectItem(root, "alias");
    const char *alias_str = cJSON_GetStringValue(got_alias);
    char *serialized = cJSON_PrintUnformatted(root);
    (void)field_str;
    (void)alias_str;

    // step 4: Cleanup
    cJSON_free(serialized);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}