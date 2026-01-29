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
//<ID> 1529
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
    cJSON *name_item = cJSON_CreateString("Alice");
    cJSON *age_item = cJSON_CreateNumber(30.0);
    cJSON_bool added_name = cJSON_AddItemToObject(person, "name", name_item);
    cJSON_bool added_age = cJSON_AddItemToObject(person, "age", age_item);
    cJSON_bool added_person = cJSON_AddItemToObject(root, "person", person);
    cJSON *list = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.0);
    cJSON_bool added_num0 = cJSON_AddItemToArray(list, num0);
    cJSON_bool added_num1 = cJSON_AddItemToArray(list, num1);
    cJSON_bool added_list = cJSON_AddItemToObject(root, "list", list);
    cJSON *active_bool = cJSON_CreateBool(1);
    cJSON_bool added_active = cJSON_AddItemToObject(root, "active", active_bool);
    (void)added_name; (void)added_age; (void)added_person; (void)added_num0; (void)added_num1; (void)added_list; (void)added_active;

    // step 2: Configure
    cJSON *replacement_number = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(list, 1, replacement_number);
    (void)replaced;

    // step 3: Operate & Validate
    cJSON *person_name = cJSON_GetObjectItem(person, "name");
    cJSON *person_age = cJSON_GetObjectItem(person, "age");
    cJSON *list_item_1 = cJSON_GetArrayItem(list, 1);
    cJSON_bool name_is_string = cJSON_IsString(person_name);
    cJSON_bool age_is_number = cJSON_IsNumber(person_age);
    cJSON_bool list_item_1_is_number = cJSON_IsNumber(list_item_1);
    double age_value = cJSON_GetNumberValue(person_age);
    double list_val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(list, 0));
    char *printed = cJSON_PrintUnformatted(root);
    (void)name_is_string; (void)age_is_number; (void)list_item_1_is_number; (void)age_value; (void)list_val0;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}