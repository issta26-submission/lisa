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
//<ID> 1291
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
    cJSON_AddItemToObject(user, "name", name);
    cJSON_AddItemToObject(user, "active", active);
    cJSON_AddItemToObject(root, "user", user);
    cJSON *id = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "id", id);
    cJSON *notes = cJSON_CreateString("sample");
    cJSON_AddItemToObject(root, "notes", notes);

    // step 2: Configure
    cJSON *user_copy = cJSON_Duplicate(user, 1);
    cJSON_AddItemToObject(root, "user_copy", user_copy);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(user, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *active_item = cJSON_GetObjectItem(user, "active");
    cJSON_bool is_bool = cJSON_IsBool(active_item);
    cJSON_bool is_false = cJSON_IsFalse(active_item);
    cJSON_bool compare_same = cJSON_Compare(user, user_copy, 1);
    double computed_val = (double)is_bool + (double)is_false + (double)compare_same;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}