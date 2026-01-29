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
//<ID> 1272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *names = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("Alice");
    cJSON *s2 = cJSON_CreateString("Bob");

    // step 2: Configure
    cJSON_AddItemToArray(names, s1);
    cJSON_AddItemToArray(names, s2);
    cJSON_AddItemToObject(root, "names", names);
    cJSON_AddNullToObject(meta, "deprecated");
    cJSON_AddObjectToObject(meta, "details");

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(names);
    cJSON_AddNumberToObject(meta, "count", (double)count);
    cJSON *first = cJSON_GetArrayItem(names, 0);
    const char *first_name = cJSON_GetStringValue(first);
    cJSON_AddStringToObject(meta, "first_name", first_name);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}