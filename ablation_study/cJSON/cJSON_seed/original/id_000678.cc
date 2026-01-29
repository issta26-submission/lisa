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
//<ID> 678
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
    cJSON_AddItemToObject(root, "person", person);
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "age", age);
    cJSON *tags = cJSON_CreateArray();
    cJSON_AddItemToObject(person, "tags", tags);
    cJSON *tag0 = cJSON_CreateString("engineer");
    cJSON *tag1 = cJSON_CreateString("team lead");
    cJSON_AddItemToArray(tags, tag0);
    cJSON_AddItemToArray(tags, tag1);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(person, "active", active);

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("Alice Smith");
    cJSON_ReplaceItemInObject(person, "name", new_name);

    // step 3: Operate & Validate
    cJSON *name_after = cJSON_GetObjectItem(person, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_after);
    cJSON_AddBoolToObject(person, "name_is_string", name_is_string);
    cJSON *active_item = cJSON_GetObjectItem(person, "active");
    cJSON_bool active_is_bool = cJSON_IsBool(active_item);
    cJSON_AddBoolToObject(person, "active_is_bool", active_is_bool);
    int tags_count = cJSON_GetArraySize(tags);
    cJSON_AddNumberToObject(person, "tags_count", (double)tags_count);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}