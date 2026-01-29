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
//<ID> 190
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
    cJSON_AddItemToObject(root, "user", user);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(user, "age", age);
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(user, "name", name);

    // step 2: Configure
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(unformatted);
    void *tmp_buf = cJSON_malloc(128);
    memset(tmp_buf, 0, 128);
    cJSON *parsed = cJSON_Parse(unformatted);

    // step 3: Operate and Validate
    cJSON *user_parsed = cJSON_GetObjectItemCaseSensitive(parsed, "user");
    cJSON *name_parsed = cJSON_GetObjectItemCaseSensitive(user_parsed, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_parsed);
    cJSON *age_parsed = cJSON_GetObjectItemCaseSensitive(user_parsed, "age");
    double age_value = cJSON_GetNumberValue(age_parsed);
    cJSON *new_name = cJSON_CreateString("Bob");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(user_parsed, "name", new_name);
    int summary = (int)name_is_string + (int)replaced + (int)age_value + parsed->type + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(tmp_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}