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
//<ID> 481
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *s1 = cJSON_CreateString("alpha");
    const char *beta_lit = "beta";
    cJSON *s2_ref = cJSON_CreateStringReference(beta_lit);
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2_ref);
    cJSON_AddItemToArray(items, s3);

    // step 2: Configure
    cJSON *it0 = cJSON_GetArrayItem(items, 0);
    cJSON *it1 = cJSON_GetArrayItem(items, 1);
    cJSON *it2 = cJSON_GetArrayItem(items, 2);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    char *v0 = cJSON_GetStringValue(it0);
    char *v1 = cJSON_GetStringValue(it1);
    char *v2 = cJSON_GetStringValue(it2);
    sprintf(buffer, "%s|%s|%s", v0 ? v0 : "null", v1 ? v1 : "null", v2 ? v2 : "null");
    cJSON *combined = cJSON_CreateString(buffer);
    cJSON_ReplaceItemInArray(items, 1, combined);

    // step 3: Operate & Validate
    cJSON *new_it1 = cJSON_GetArrayItem(items, 1);
    char *new_val = cJSON_GetStringValue(new_it1);
    cJSON *snapshot = cJSON_Duplicate(root, 1);
    cJSON_bool same = cJSON_Compare(root, snapshot, 1);
    (void)new_val;
    (void)same;

    // step 4: Cleanup
    cJSON_Delete(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}