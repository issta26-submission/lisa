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
//<ID> 766
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
    cJSON *items = (cJSON *)0;
    cJSON *name = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    char *second_str = (char *)0;
    cJSON_bool person_is_object = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    items = cJSON_CreateArray();
    name = cJSON_CreateString("Bob");

    // step 3: Configure
    cJSON_AddItemToObject(root, "person", person);
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateString("first"));
    cJSON_AddItemToArray(items, cJSON_CreateString("second"));

    // step 4: Operate
    second_item = cJSON_GetArrayItem(items, 1);
    second_str = cJSON_GetStringValue(second_item);
    person_is_object = cJSON_IsObject(person);

    // step 5: Validate
    validation_score = (int)person_is_object + (second_str != (char *)0) + (cJSON_GetStringValue(name) != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}