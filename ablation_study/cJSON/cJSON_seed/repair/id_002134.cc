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
//<ID> 2134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *entries = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "entries", entries);
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello_world");
    cJSON *n = cJSON_CreateNull();
    cJSON_AddItemToArray(entries, n);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(entries, s1);
    cJSON_AddItemToArray(entries, s2);
    cJSON *first_item = cJSON_GetArrayItem(entries, 0);
    cJSON_SetValuestring(first_item, "updated_alpha");

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(entries, 1);
    cJSON_AddNumberToObject(root, "entries_count", (double)cJSON_GetArraySize(entries));
    cJSON_AddStringToObject(root, "greeting_copy", cJSON_GetStringValue(greeting));

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}