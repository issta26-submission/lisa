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
//<ID> 1517
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
    cJSON_AddStringToObject(person, "name", "Bob");
    cJSON_AddNumberToObject(person, "age", 28.0);
    cJSON *nickname = cJSON_CreateNull();
    cJSON_AddItemToObject(person, "nickname", nickname);
    cJSON_AddItemToObject(root, "person", person);
    cJSON *temp = cJSON_CreateString("temporary");
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    cJSON *skills = cJSON_CreateArray();
    cJSON_AddItemToArray(skills, cJSON_CreateString("C"));
    cJSON_AddItemToArray(skills, cJSON_CreateString("C++"));
    cJSON_AddItemToObject(root, "skills", skills);
    cJSON *status = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "status", status);

    // step 3: Operate & Validate
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON_bool has_missing = cJSON_HasObjectItem(root, "missing");
    cJSON *nickname_item = cJSON_GetObjectItem(person, "nickname");
    cJSON_bool nickname_is_null = cJSON_IsNull(nickname_item);
    cJSON_AddBoolToObject(status, "has_person", has_person);
    cJSON_AddBoolToObject(status, "has_missing", has_missing);
    cJSON_AddBoolToObject(status, "nickname_is_null", nickname_is_null);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    cJSON_bool has_temp_after_delete = cJSON_HasObjectItem(root, "temp");
    cJSON_AddBoolToObject(status, "has_temp_after_delete", has_temp_after_delete);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}