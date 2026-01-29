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
//<ID> 1204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"flag\":false,\"nested\":{\"val\":123.0}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = NULL;
    cJSON *flag = NULL;
    cJSON *nested = NULL;
    cJSON *nested_dup = NULL;
    cJSON *true_item = NULL;
    char *printed = NULL;
    cJSON_bool was_false = 0;
    double nested_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLength(json, json_len);
    flag = cJSON_GetObjectItem(root, "flag");
    nested = cJSON_GetObjectItem(root, "nested");
    was_false = cJSON_IsFalse(flag);
    nested_dup = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "nested_ref", nested_dup);
    true_item = cJSON_CreateTrue();
    cJSON_ReplaceItemInObject(root, "flag", true_item);

    // step 3: Operate / Validate
    nested_val = cJSON_GetNumberValue(cJSON_GetObjectItem(nested, "val"));
    printed = cJSON_PrintUnformatted(root);
    (void)was_false;
    (void)nested_val;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
}