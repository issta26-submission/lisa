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
//<ID> 1091
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON *activeFalse = cJSON_CreateFalse();
    cJSON *nickname = cJSON_CreateString("Ally");

    // step 2: Configure
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(person, "age", age);
    cJSON_AddItemToObject(person, "active", activeFalse);
    cJSON_AddItemToObject(person, "nickname", nickname);
    cJSON_AddItemToObject(root, "person", person);

    // step 3: Operate & Validate
    cJSON *nameItem = cJSON_GetObjectItem(person, "name");
    const char *name_str = cJSON_GetStringValue(nameItem);
    cJSON *copied_name = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "copied_name", copied_name);
    cJSON *newNick = cJSON_CreateString("A.");
    cJSON_bool replaced_nick = cJSON_ReplaceItemInObject(person, "nickname", newNick);
    cJSON *newTrue = cJSON_CreateTrue();
    cJSON_bool replaced_active = cJSON_ReplaceItemViaPointer(person, activeFalse, newTrue);
    cJSON *nick_after = cJSON_GetObjectItem(person, "nickname");
    const char *nick_val = cJSON_GetStringValue(nick_after);
    (void)nick_val;
    (void)replaced_nick;
    (void)replaced_active;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}