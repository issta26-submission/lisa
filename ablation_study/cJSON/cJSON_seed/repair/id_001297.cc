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
//<ID> 1297
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
    cJSON *active_true = cJSON_CreateTrue();
    cJSON *active_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(user, "name", name);
    cJSON_AddItemToObject(user, "active_true", active_true);
    cJSON_AddItemToObject(user, "active_false", active_false);
    cJSON_AddItemToObject(root, "user", user);
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *user_copy = cJSON_Duplicate(user, 1);
    cJSON_AddItemToObject(root, "user_copy", user_copy);
    cJSON *replacement_name = cJSON_CreateString("Carol");
    cJSON_ReplaceItemInObject(user, "name", replacement_name);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(user, "name");
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON *af_item = cJSON_GetObjectItem(user, "active_false");
    cJSON_bool af_is_bool = cJSON_IsBool(af_item);
    cJSON_bool af_is_false = cJSON_IsFalse(af_item);
    cJSON_bool users_equal = cJSON_Compare(user, user_copy, 1);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    ((char *)scratch)[2] = (name_val && name_val[0]) ? name_val[0] : '\0';
    ((char *)scratch)[3] = (name_val && name_val[1]) ? name_val[1] : '\0';
    (void)af_is_bool;
    (void)af_is_false;
    (void)users_equal;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}