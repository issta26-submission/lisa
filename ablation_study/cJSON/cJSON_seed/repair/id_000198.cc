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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *people = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "people", people);

    // step 2: Configure
    cJSON *person1 = cJSON_CreateObject();
    cJSON_AddStringToObject(person1, "name", "Alice");
    cJSON *role1 = cJSON_CreateString("admin");
    cJSON_AddItemToObject(person1, "role", role1);
    cJSON_AddItemToArray(people, person1);

    cJSON *person2 = cJSON_CreateObject();
    cJSON_AddStringToObject(person2, "name", "Bob");
    cJSON *role2 = cJSON_CreateString("user");
    cJSON_AddItemToObject(person2, "role", role2);
    cJSON_AddItemToArray(people, person2);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(people);
    cJSON *first = cJSON_GetArrayItem(people, 0);
    cJSON *first_name_item = cJSON_GetObjectItem(first, "name");
    const char *first_name = cJSON_GetStringValue(first_name_item);
    cJSON *first_name_copy = cJSON_CreateString(first_name);
    cJSON_AddItemToObject(root, "first_name_copy", first_name_copy);
    (void)count; // used to show data flow (array size obtained and accessible)

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}