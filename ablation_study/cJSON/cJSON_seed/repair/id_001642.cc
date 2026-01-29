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
//<ID> 1642
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "field", orig);
    cJSON_AddItemToObject(root, "inner", inner);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(inner, "is_active", flag_true);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replacement_value");
    char *set_res = cJSON_SetValuestring(replacement, "replacement_value_updated");
    (void)set_res;
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "field", replacement);

    // step 3: Operate and Validate
    cJSON *found = cJSON_GetObjectItem(root, "field");
    char *found_str = cJSON_GetStringValue(found);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = found_str[0];
    buffer[1] = (char)('0' + (int)replaced);
    buffer[2] = printed[0];
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}