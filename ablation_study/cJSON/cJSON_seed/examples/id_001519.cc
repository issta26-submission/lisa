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
//<ID> 1519
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
    cJSON_AddItemToObject(root, "person", person);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToArray(list, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(2.0));
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    char *printed_root = cJSON_PrintUnformatted(root);
    cJSON *maybe_item = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool maybe_is_null = cJSON_IsNull(maybe_item);
    cJSON_bool has_person_before = cJSON_HasObjectItem(root, "person");
    cJSON *person_item = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person_item, "name");
    const char *name_value = cJSON_GetStringValue(name_item);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "maybe");
    cJSON_bool has_maybe_after = cJSON_HasObjectItem(root, "maybe");
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool equal_structs = cJSON_Compare(root, duplicate, 1);
    char *printed_dup = cJSON_PrintUnformatted(duplicate);
    (void)name_value;
    (void)maybe_is_null;
    (void)has_person_before;
    (void)has_maybe_after;
    (void)equal_structs;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}