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
//<ID> 842
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *friend_item = cJSON_CreateString("Bob");

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "Alice");
    cJSON_AddItemToObject(root, "friend", friend_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON *greeting = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}