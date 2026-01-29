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
//<ID> 1290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *user = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(user, "name", name);
    cJSON *active = cJSON_CreateFalse();
    cJSON_AddItemToObject(user, "active", active);
    cJSON_AddItemToObject(root, "user", user);
    cJSON *scores = cJSON_CreateArray();
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(100.0));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(85.5));
    cJSON_AddItemToObject(root, "scores", scores);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *out = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    const char *name_str = cJSON_GetStringValue(name);
    cJSON_bool is_bool = cJSON_IsBool(active);
    cJSON_bool is_false = cJSON_IsFalse(active);
    cJSON_bool same = cJSON_Compare(root, dup, 1);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = name_str ? name_str[0] : '\0';
    ((char *)scratch)[1] = out[0];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}