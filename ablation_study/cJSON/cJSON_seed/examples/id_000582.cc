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
//<ID> 582
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *person = cJSON_CreateObject();
    cJSON_AddStringToObject(person, "name", "Alice");
    cJSON_AddNumberToObject(person, "age", 30.0);
    const char *hobbies_arr[] = { "reading", "hiking", "gaming" };
    cJSON *hobbies = cJSON_CreateStringArray(hobbies_arr, 3);
    cJSON_AddItemToObject(person, "hobbies", hobbies);
    cJSON_AddItemToObject(root, "person", person);

    // step 3: Operate & Validate
    int hobby_count = cJSON_GetArraySize(hobbies);
    cJSON *dup_person = cJSON_Duplicate(person, 1);
    cJSON_bool added_cs = cJSON_AddItemToObjectCS(root, "person_copy", dup_person);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_person = cJSON_GetObjectItem(parsed, "person");
    cJSON *parsed_name_item = cJSON_GetObjectItem(parsed_person, "name");
    const char *parsed_name = cJSON_GetStringValue(parsed_name_item);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;hobbies=%d;name=%s",
            version ? version : "null",
            hobby_count,
            parsed_name ? parsed_name : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}