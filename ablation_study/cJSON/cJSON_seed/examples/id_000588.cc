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
//<ID> 588
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
    cJSON *child = cJSON_CreateObject();

    // step 2: Configure
    cJSON *title = cJSON_AddStringToObject(root, "title", "Demo Document");
    cJSON *id_item = cJSON_AddNumberToObject(root, "id", 42.0);
    cJSON_bool added_child = cJSON_AddItemToObjectCS(root, "child_cs", child);
    cJSON *child_name = cJSON_AddStringToObject(child, "child_name", "child1");
    cJSON *child_dup = cJSON_Duplicate(child, 1);
    cJSON_bool added_child_dup = cJSON_AddItemToObjectCS(root, "child_dup", child_dup);

    // step 3: Operate & Validate
    const char *names_arr[] = { "alice", "bob", "carol" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    int names_count = cJSON_GetArraySize(names);
    char *snapshot = cJSON_PrintUnformatted(root);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;id=%.0f;names_count=%d",
            version ? version : "null",
            cJSON_GetNumberValue(id_item),
            names_count);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}