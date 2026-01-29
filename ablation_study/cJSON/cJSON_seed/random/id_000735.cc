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
//<ID> 735
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
    cJSON *hobbies = (cJSON *)0;
    cJSON *name = (cJSON *)0;
    cJSON *age = (cJSON *)0;
    cJSON *hobby1 = (cJSON *)0;
    cJSON *hobby2 = (cJSON *)0;
    cJSON *detached_age = (cJSON *)0;
    cJSON *got_name = (cJSON *)0;
    cJSON *got_person = (cJSON *)0;
    cJSON_bool added_person = 0;
    cJSON_bool added_name = 0;
    cJSON_bool added_age = 0;
    cJSON_bool added_hobbies = 0;
    cJSON_bool array_added1 = 0;
    cJSON_bool array_added2 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    hobbies = cJSON_CreateArray();
    name = cJSON_CreateString("Bob");
    age = cJSON_CreateNumber(42.0);
    hobby1 = cJSON_CreateString("chess");
    hobby2 = cJSON_CreateString("golf");

    // step 3: Configure
    array_added1 = cJSON_AddItemToArray(hobbies, hobby1);
    array_added2 = cJSON_AddItemToArray(hobbies, hobby2);
    added_name = cJSON_AddItemToObject(person, "name", name);
    added_age = cJSON_AddItemToObject(person, "age", age);
    added_hobbies = cJSON_AddItemToObject(person, "hobbies", hobbies);
    added_person = cJSON_AddItemToObject(root, "person", person);

    // step 4: Operate
    detached_age = cJSON_DetachItemFromObject(person, "age");
    got_name = cJSON_GetObjectItem(person, "name");
    got_person = cJSON_GetObjectItem(root, "person");

    // step 5: Validate
    validation_score = (int)array_added1 + (int)array_added2 + (int)added_name + (int)added_age + (int)added_hobbies + (int)added_person
                     + (got_name != (cJSON *)0) + (got_person != (cJSON *)0) + (detached_age != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(detached_age);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}