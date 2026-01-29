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
//<ID> 787
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json = "{\"user\":{\"name\":\"alice\",\"age\":30},\"active\":true}";
    cJSON *parsed = cJSON_Parse(json);

    // step 2: Configure
    cJSON *user = cJSON_DetachItemFromObject(parsed, "user");
    cJSON_AddItemToObject(root, "source_parsed", parsed);
    cJSON_AddItemToObject(root, "imported_user", user);
    cJSON_AddStringToObject(user, "role", "admin");
    cJSON_AddNumberToObject(user, "score", 99.5);
    cJSON_AddNumberToObject(root, "item_count", 2.0);
    cJSON_AddStringToObject(root, "note", "imported");

    // step 3: Operate and Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}