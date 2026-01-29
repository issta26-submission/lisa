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
//<ID> 739
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *person = (cJSON *)0;
    cJSON *list = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *age_item = (cJSON *)0;
    cJSON *retrieved_person = (cJSON *)0;
    cJSON *retrieved_age = (cJSON *)0;
    cJSON *detached_age = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_age = 0;
    cJSON_bool added_person = 0;
    cJSON_bool added_list = 0;
    cJSON_bool added_to_array = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    list = cJSON_CreateArray();
    name_item = cJSON_CreateString("Bob");
    age_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_name = cJSON_AddItemToObject(person, "name", name_item);
    added_age = cJSON_AddItemToObject(person, "age", age_item);
    added_person = cJSON_AddItemToObject(root, "person", person);
    added_list = cJSON_AddItemToObject(root, "list", list);

    // step 4: Operate
    retrieved_person = cJSON_GetObjectItem(root, "person");
    retrieved_age = cJSON_GetObjectItem(retrieved_person, "age");
    detached_age = cJSON_DetachItemFromObject(retrieved_person, "age");
    added_to_array = cJSON_AddItemToArray(list, detached_age);
    (void)retrieved_age;

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (printed != (char *)0) + (int)added_name + (int)added_age + (int)added_person + (int)added_list + (int)added_to_array;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}