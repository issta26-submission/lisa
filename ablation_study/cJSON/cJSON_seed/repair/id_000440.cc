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
//<ID> 440
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
    cJSON_AddItemToObject(root, "tags", tags);
    cJSON_AddItemToArray(tags, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("beta"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("gamma"));

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(person, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON *email_item = cJSON_GetObjectItem(person, "email");
    cJSON_bool email_is_string = cJSON_IsString(email_item);
    (void)name_is_string;
    (void)email_is_string;

    // step 3: Operate
    cJSON_DeleteItemFromArray(tags, 0);
    cJSON_AddItemToArray(tags, cJSON_CreateString("delta"));
    cJSON *tags_ref = cJSON_GetObjectItem(root, "tags");
    cJSON *second_tag = cJSON_GetArrayItem(tags_ref, 1);
    cJSON_bool second_is_string = cJSON_IsString(second_tag);
    (void)second_is_string;

    // step 4: Validate and Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}