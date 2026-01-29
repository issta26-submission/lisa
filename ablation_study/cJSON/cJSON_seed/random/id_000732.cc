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
//<ID> 732
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
    cJSON *name_item = (cJSON *)0;
    cJSON *age_item = (cJSON *)0;
    cJSON *list = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *detached_person = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON_bool added_person = 0;
    cJSON_bool added_list = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_detached = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    name_item = cJSON_CreateString("Charlie");
    age_item = cJSON_CreateNumber(28);
    list = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(100);
    num2 = cJSON_CreateNumber(200);

    // step 3: Configure
    cJSON_AddItemToObject(person, "name", name_item);
    cJSON_AddItemToObject(person, "age", age_item);
    added_person = cJSON_AddItemToObject(root, "person", person);
    added_list = cJSON_AddItemToObject(root, "list", list);
    added_num1 = cJSON_AddItemToArray(list, num1);
    added_num2 = cJSON_AddItemToArray(list, num2);

    // step 4: Operate
    detached_person = cJSON_DetachItemFromObject(root, "person");
    added_detached = cJSON_AddItemToArray(list, detached_person);
    retrieved_list = cJSON_GetObjectItem(root, "list");
    retrieved_name = cJSON_GetObjectItem(detached_person, "name");

    // step 5: Validate
    validation_score = (int)added_person + (int)added_list + (int)added_num1 + (int)added_num2 + (int)added_detached
                     + (retrieved_list != (cJSON *)0) + (retrieved_name != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}