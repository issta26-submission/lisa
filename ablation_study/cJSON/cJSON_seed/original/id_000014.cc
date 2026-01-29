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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"fuzz\",\"enabled\":false,\"count\":3}";
    const char *parse_end = nullptr;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON_AddTrueToObject(root_dup, "processed");
    cJSON_AddFalseToObject(root_dup, "flagged");
    cJSON *name = cJSON_GetObjectItem(root, "name");
    cJSON *name_dup = cJSON_Duplicate(name, 0);
    cJSON_AddItemToObject(root_dup, "nameCopy", name_dup);

    // step 3: Operate and Validate
    char *serialized = cJSON_PrintUnformatted(root_dup);
    cJSON *reparsed = cJSON_ParseWithOpts(serialized, nullptr, 1);
    cJSON *reparsed_dup = cJSON_Duplicate(reparsed, 1);

    // step 4: Cleanup
    cJSON_Delete(reparsed_dup);
    cJSON_Delete(reparsed);
    cJSON_free(serialized);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}