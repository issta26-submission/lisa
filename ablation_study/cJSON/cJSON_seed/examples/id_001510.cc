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
//<ID> 1510
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
    const int nums[4] = {10, 20, 30, 40};
    cJSON *numbers = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    // Use a safe duplicate instead of a direct object reference to avoid
    // potential double-free or linked-list corruption when deleting later.
    cJSON *person_ref = cJSON_Duplicate(person, 1);
    cJSON_AddItemToObject(root, "person_ref", person_ref);
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);

    // step 3: Operate & Validate
    cJSON *maybe_item = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool is_null = cJSON_IsNull(maybe_item);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool copy_has_numbers = cJSON_HasObjectItem(copy, "numbers");
    (void)has_person;
    (void)is_null;
    (void)copy_has_numbers;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "person_ref");
    cJSON_free(printed);
    cJSON_Delete(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}