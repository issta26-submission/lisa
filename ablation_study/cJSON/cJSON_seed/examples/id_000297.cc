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
//<ID> 297
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
    cJSON *age = cJSON_CreateNumber(30);
    cJSON *hobbies = cJSON_CreateArray();
    cJSON *hb1 = cJSON_CreateString("reading");
    cJSON *hb2 = cJSON_CreateString("cycling");

    // step 2: Configure
    cJSON_AddItemToArray(hobbies, hb1);
    cJSON_AddItemToArray(hobbies, hb2);
    cJSON_AddItemToObjectCS(person, "name", name);
    cJSON_AddItemToObjectCS(person, "age", age);
    cJSON_AddItemToObjectCS(person, "hobbies", hobbies);
    cJSON_AddItemToObjectCS(root, "person", person);

    // step 3: Operate & Validate
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON *has_person_flag = cJSON_CreateBool(has_person);
    cJSON_AddItemToObjectCS(root, "has_person", has_person_flag);
    cJSON *person_ptr = cJSON_GetObjectItem(root, "person");
    cJSON *name_ptr = cJSON_GetObjectItem(person_ptr, "name");
    char *name_text = cJSON_GetStringValue(name_ptr);
    cJSON *name_copy = cJSON_CreateString(name_text);
    cJSON_AddItemToObjectCS(root, "person_name_copy", name_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), (cJSON_bool)1);
    cJSON *printed_flag = cJSON_CreateBool(printed);
    cJSON_AddItemToObjectCS(root, "printed_ok", printed_flag);
    cJSON_DeleteItemFromObjectCaseSensitive(person, "age");
    cJSON_bool has_age = cJSON_HasObjectItem(person, "age");
    cJSON *has_age_flag = cJSON_CreateBool(has_age);
    cJSON_AddItemToObjectCS(root, "age_present_after_delete", has_age_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}