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
//<ID> 1076
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *b_true = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, b_true);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "owner", "tester");
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate & Validate
    cJSON *items_from_root = cJSON_GetObjectItem(root, "items");
    cJSON *owner_from_root = cJSON_GetObjectItem(root, "owner");
    const char *owner_name = cJSON_GetStringValue(owner_from_root);
    cJSON *active_from_root = cJSON_GetObjectItem(root, "active");
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)items_from_root;
    (void)owner_name;
    (void)active_from_root;
    return 66;
}