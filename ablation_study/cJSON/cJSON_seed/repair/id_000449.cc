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
//<ID> 449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *users = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "users", users);
    cJSON *user1 = cJSON_CreateObject();
    cJSON_AddStringToObject(user1, "name", "Alice");
    cJSON_AddStringToObject(user1, "email", "alice@example.com");
    cJSON_AddItemToArray(users, user1);
    cJSON *user2 = cJSON_CreateObject();
    cJSON_AddStringToObject(user2, "name", "Bob");
    cJSON_AddStringToObject(user2, "email", "bob@example.com");
    cJSON_AddItemToArray(users, user2);

    // step 2: Configure
    cJSON_AddStringToObject(root, "env", "production");
    cJSON_AddStringToObject(root, "version", "1.0");

    // step 3: Operate
    cJSON *users_from_root = cJSON_GetObjectItem(root, "users");
    cJSON *first_user = cJSON_GetArrayItem(users_from_root, 0);
    cJSON *email_item = cJSON_GetObjectItem(first_user, "email");
    cJSON_bool email_is_string = cJSON_IsString(email_item);
    const char *email_value = cJSON_GetStringValue(email_item);
    (void)email_is_string;
    (void)email_value;
    cJSON_DeleteItemFromArray(users_from_root, 1);

    // step 4: Validate and Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}