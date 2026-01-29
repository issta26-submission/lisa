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
//<ID> 357
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const names_list[] = { "alpha", "beta", "gamma" };
    cJSON *names = cJSON_CreateStringArray(names_list, 3);
    cJSON *groups = cJSON_CreateArray();
    cJSON_AddItemToArray(groups, names);
    cJSON_AddItemToObject(root, "groups", groups);

    // step 2: Configure
    int name_count = cJSON_GetArraySize(names);
    cJSON *count_holder = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(count_holder, (double)name_count);
    cJSON_AddItemToObject(root, "name_count", count_holder);
    cJSON *replacement = cJSON_CreateString("omega");
    cJSON_ReplaceItemInArray(names, 1, replacement);

    // step 3: Operate & Validate
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(summary, "primary_group", cJSON_CreateString("default"));
    cJSON_AddItemToArray(groups, summary);
    cJSON *size_check = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(size_check, (double)cJSON_GetArraySize(names));
    cJSON_AddItemToObject(root, "validated_size", size_check);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}