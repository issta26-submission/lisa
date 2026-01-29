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
//<ID> 441
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
    cJSON_AddStringToObject(person, "name", "Bob");
    cJSON_AddStringToObject(person, "email", "bob@example.com");
    cJSON *tags = cJSON_CreateArray();
    cJSON_AddItemToArray(tags, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("beta"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("gamma"));
    cJSON_AddItemToObject(root, "tags", tags);

    // step 2: Configure
    cJSON_DeleteItemFromArray(tags, 1);
    cJSON_AddItemToArray(tags, cJSON_CreateString("delta"));

    // step 3: Operate
    cJSON *person_from_root = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person_from_root, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "owner_name", name_str);
    cJSON_AddNumberToObject(root, "name_is_string", (double)name_is_str);
    cJSON *tags_from_root = cJSON_GetObjectItem(root, "tags");
    cJSON *first_tag = cJSON_GetArrayItem(tags_from_root, 0);
    cJSON_bool tag_is_str = cJSON_IsString(first_tag);
    const char *tag_str = cJSON_GetStringValue(first_tag);
    cJSON_AddStringToObject(root, "primary_tag", tag_str);
    cJSON_AddNumberToObject(root, "primary_tag_is_string", (double)tag_is_str);

    // step 4: Validate and Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}