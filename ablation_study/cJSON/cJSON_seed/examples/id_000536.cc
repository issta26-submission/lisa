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
//<ID> 536
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNullToObject(root, "optional");
    cJSON *s0 = cJSON_CreateString("first");
    cJSON *s1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);

    // step 3: Operate & Validate
    char *snapshot1 = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot1);
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON_DeleteItemFromArray(items, 0);
    char *snapshot2 = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;is_array=%d", version ? version : "null", (int)items_is_array);
    cJSON_free(snapshot1);
    cJSON_free(snapshot2);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}