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
//<ID> 294
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
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(user, "age", age);
    cJSON_AddItemToObject(user, "name", name);
    cJSON_AddItemToObjectCS(root, "user", user);
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON_bool has_user = cJSON_HasObjectItem(root, "user");
    cJSON_AddItemToObjectCS(root, "has_user", cJSON_CreateBool(has_user));
    cJSON *got_user = cJSON_GetObjectItem(root, "user");
    cJSON *got_name = cJSON_GetObjectItem(got_user, "name");
    const char *name_text = cJSON_GetStringValue(got_name);
    cJSON_AddItemToObject(meta, "name_copy", cJSON_CreateString(name_text));
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "user");
    cJSON_bool has_user_after = cJSON_HasObjectItem(root, "user");
    cJSON_AddItemToObject(root, "has_user_after_delete", cJSON_CreateBool(has_user_after));
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}