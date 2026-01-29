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
//<ID> 580
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
    const char *names_arr[] = { "alice", "bob", "carol" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *child = cJSON_CreateObject();
    cJSON *age = cJSON_AddNumberToObject(child, "age", 30.0);
    cJSON *city = cJSON_AddStringToObject(child, "city", "Metropolis");
    cJSON_AddItemToObjectCS(root, "child_cs", child);

    // step 2: Configure
    cJSON *child_dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObjectCS(root, "child_dup", child_dup);
    cJSON *score = cJSON_AddNumberToObject(root, "score", 99.5);
    cJSON *status_item = cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate & Validate
    int names_count = cJSON_GetArraySize(names);
    cJSON *names_dup = cJSON_Duplicate(names, 1);
    cJSON_AddItemToObjectCS(root, "names_copy", names_dup);
    char *snapshot = cJSON_PrintUnformatted(root);
    char report[256];
    memset(report, 0, sizeof(report));
    sprintf(report, "ver=%s;names=%d;score=%.1f;status=%s",
            version ? version : "null",
            names_count,
            99.5,
            "ok");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}