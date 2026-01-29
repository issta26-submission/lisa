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
//<ID> 793
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

    // step 2: Configure
    cJSON *entry1 = cJSON_CreateObject();
    cJSON_AddTrueToObject(entry1, "flag");
    cJSON_AddNumberToObject(entry1, "value", 42.0);
    cJSON_AddItemToArray(items, entry1);
    cJSON *entry2 = cJSON_CreateObject();
    cJSON_AddStringToObject(entry2, "name", "example");
    cJSON_AddItemToArray(items, entry2);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first);
    (void)first_invalid;
    cJSON *flag = cJSON_GetObjectItem(first, "flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    (void)flag_is_true;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}