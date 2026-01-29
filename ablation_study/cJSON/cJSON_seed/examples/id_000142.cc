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
//<ID> 142
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

    // step 2: Configure
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name);
    cJSON *middle = cJSON_CreateNull();
    cJSON_AddItemToObject(person, "middle_name", middle);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "age", age);

    // step 3: Operate & Validate
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON_bool person_is_object = cJSON_IsObject(person);
    cJSON_AddBoolToObject(root, "has_person", has_person);
    cJSON_AddBoolToObject(root, "person_is_object", person_is_object);
    cJSON *name_item = cJSON_GetObjectItem(person, "name");
    char *name_value = cJSON_GetStringValue(name_item);
    (void)name_value;
    cJSON *person_copy = cJSON_Duplicate(person, 1);
    cJSON_AddItemToObject(root, "person_copy", person_copy);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}