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
//<ID> 783
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *parsed = cJSON_Parse("{\"name\":\"alice\",\"count\":2}");
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddStringToObject(parsed, "department", "engineering");
    cJSON_AddNumberToObject(parsed, "rating", 4.7);

    // step 3: Operate and Validate
    cJSON_AddItemToObject(root, "profile", parsed);
    cJSON *detached_name = cJSON_DetachItemFromObject(parsed, "name");
    cJSON_AddItemToObject(root, "username", detached_name);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}