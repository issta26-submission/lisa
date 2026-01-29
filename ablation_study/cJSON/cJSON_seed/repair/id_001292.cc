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
//<ID> 1292
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
    cJSON *active = cJSON_CreateFalse();
    cJSON *scores = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateNumber(75.0);
    cJSON *s2 = cJSON_CreateNumber(88.5);
    cJSON_AddItemToArray(scores, s1);
    cJSON_AddItemToArray(scores, s2);
    cJSON_AddItemToObject(user, "name", name);
    cJSON_AddItemToObject(root, "user", user);
    cJSON_AddItemToObject(root, "active", active);
    cJSON_AddItemToObject(root, "scores", scores);
    cJSON *status = cJSON_CreateString("inactive");
    cJSON_AddItemToObject(root, "status", status);

    // step 2: Configure
    cJSON *replacement_status = cJSON_CreateString("active");
    cJSON_ReplaceItemViaPointer(root, status, replacement_status);
    cJSON *name_copy = cJSON_CreateString("Alice");

    // step 3: Operate and Validate
    cJSON *user_obj = cJSON_GetObjectItem(root, "user");
    cJSON *name_item = cJSON_GetObjectItem(user_obj, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON_bool is_bool = cJSON_IsBool(active);
    cJSON_bool is_false = cJSON_IsFalse(active);
    cJSON_bool equal_name = cJSON_Compare(name_item, name_copy, 1);
    double computed_val = ((double)(is_bool + is_false + equal_name) * 2.5) + (double)(name_str ? name_str[0] : 0);
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out ? out[0] : '\0';
    ((char *)scratch)[1] = name_str ? name_str[0] : '\0';

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(name_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}