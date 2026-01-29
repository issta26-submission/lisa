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
//<ID> 1097
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
    cJSON *name = cJSON_CreateString("alice");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON *active_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(person, "age", age);
    cJSON_AddItemToObject(person, "active", active_true);
    cJSON_AddItemToObject(root, "person", person);

    // step 2: Configure
    const char *name_str = cJSON_GetStringValue(name);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "person_name_copy", name_copy);

    // step 3: Operate & Validate
    cJSON_bool replaced_active = cJSON_ReplaceItemInObject(person, "active", cJSON_CreateFalse());
    cJSON *copy_ptr = cJSON_GetObjectItem(root, "person_name_copy");
    cJSON *new_copy = cJSON_CreateString(name_str);
    cJSON_bool replaced_copy = cJSON_ReplaceItemViaPointer(root, copy_ptr, new_copy);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)replaced_active;
    (void)replaced_copy;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}