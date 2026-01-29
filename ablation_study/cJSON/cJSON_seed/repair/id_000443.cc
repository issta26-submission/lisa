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
//<ID> 443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "title", "ExampleDocument");
    cJSON *person = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "person", person);
    cJSON_AddStringToObject(person, "name", "Bob");
    cJSON_AddStringToObject(person, "email", "bob@example.com");
    cJSON *tags = cJSON_CreateArray();
    cJSON_AddItemToObject(person, "tags", tags);
    cJSON *tag1 = cJSON_CreateString("alpha");
    cJSON *tag2 = cJSON_CreateString("beta");
    cJSON *tag3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(tags, tag1);
    cJSON_AddItemToArray(tags, tag2);
    cJSON_AddItemToArray(tags, tag3);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "active");
    cJSON_AddStringToObject(root, "owner", "team@example.com");

    // step 3: Operate
    cJSON *person_from_root = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person_from_root, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    cJSON_AddNumberToObject(root, "name_is_string", (double)name_is_str);
    cJSON_DeleteItemFromArray(tags, 1);
    int remaining = cJSON_GetArraySize(tags);
    cJSON_AddNumberToObject(root, "tag_count", (double)remaining);

    // step 4: Validate & Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}