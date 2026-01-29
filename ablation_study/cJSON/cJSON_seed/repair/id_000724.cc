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
//<ID> 724
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *s0 = cJSON_CreateString("first");
    cJSON_AddItemToArray(items, s0);
    cJSON *modifiable = cJSON_CreateString("to_be_changed");
    cJSON_AddItemToArray(items, modifiable);

    // step 3: Operate and Validate
    char *new_val = cJSON_SetValuestring(modifiable, "changed_value");
    double first_char_code = (double)(unsigned char)new_val[0];
    cJSON_AddNumberToObject(root, "first_char_code", first_char_code);
    cJSON *items_ref = cJSON_GetObjectItemCaseSensitive(root, "items");
    int total = cJSON_GetArraySize(items_ref);
    cJSON_AddNumberToObject(root, "count", (double)total);
    cJSON *flag_ref = cJSON_GetObjectItemCaseSensitive(root, "flag");
    double flag_is_false = (double)cJSON_IsFalse(flag_ref);
    cJSON_AddNumberToObject(root, "flag_is_false", flag_is_false);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}