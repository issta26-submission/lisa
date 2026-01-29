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
//<ID> 768
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
    cJSON *arr = (cJSON *)0;
    cJSON *arr_item0 = (cJSON *)0;
    cJSON *arr_item1 = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    cJSON *copied_item = (cJSON *)0;
    char *retrieved_str = (char *)0;
    cJSON_bool person_is_object = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    name_item = cJSON_CreateString("Bob");
    arr = cJSON_CreateArray();
    arr_item0 = cJSON_CreateString("zero");
    arr_item1 = cJSON_CreateString("one");

    // step 3: Configure
    cJSON_AddItemToObject(root, "person", person);
    cJSON_AddItemToObject(person, "name", name_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, arr_item0);
    cJSON_AddItemToArray(arr, arr_item1);

    // step 4: Operate
    retrieved_item = cJSON_GetArrayItem(arr, 1);
    retrieved_str = cJSON_GetStringValue(retrieved_item);
    copied_item = cJSON_CreateString(retrieved_str);
    cJSON_AddItemToObject(root, "second_item_copy", copied_item);
    person_is_object = cJSON_IsObject(person);

    // step 5: Validate
    validation_score = (retrieved_item != (cJSON *)0) + (retrieved_str != (char *)0) + (int)person_is_object;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}