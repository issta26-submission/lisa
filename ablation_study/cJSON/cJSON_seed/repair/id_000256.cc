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
//<ID> 256
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *names = cJSON_AddArrayToObject(root, "names");
    cJSON *s1 = cJSON_CreateString("Alice");
    cJSON *s2 = cJSON_CreateString("Bob");
    cJSON_AddItemToArray(names, s1);
    cJSON_AddItemToArray(names, s2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON *source = cJSON_CreateString("generated");
    cJSON_AddItemToObjectCS(meta, "source", source);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(names, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObjectCS(meta, "first_copy", first_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}