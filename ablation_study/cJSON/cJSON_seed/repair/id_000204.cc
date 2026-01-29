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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *empty = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", empty);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s2);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(s1, 1);
    cJSON_AddItemToArray(items, dup);

    // step 3: Operate and Validate
    cJSON *retrieved_items = cJSON_GetObjectItem(root, "items");
    cJSON *first = cJSON_GetArrayItem(retrieved_items, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON_AddStringToObject(root, "first_copy", first_val);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool was_false = cJSON_IsFalse(flag_item);
    (void)was_false;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}