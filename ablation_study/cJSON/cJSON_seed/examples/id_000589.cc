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
//<ID> 589
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
    cJSON *names = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateString("alice");
    cJSON_AddItemToArray(names, n1);
    cJSON *n2 = cJSON_CreateString("bob");
    cJSON_AddItemToArray(names, n2);
    cJSON_AddItemToObject(root, "names", names);

    // step 2: Configure
    int size_original = cJSON_GetArraySize(names);
    cJSON_AddNumberToObject(root, "count", (double)size_original);
    cJSON_AddStringToObject(root, "owner", "tester");

    // step 3: Operate & Validate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_names = cJSON_GetObjectItem(dup_root, "names");
    int dup_size = cJSON_GetArraySize(dup_names);
    cJSON_AddItemToObjectCS(root, "duplicate", dup_root);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;orig=%d;dup=%d;owner=%s",
            version ? version : "null",
            size_original,
            dup_size,
            cJSON_GetStringValue(cJSON_GetObjectItem(root, "owner")));
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}