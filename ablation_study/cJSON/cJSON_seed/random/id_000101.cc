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
//<ID> 101
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
    cJSON *name = (cJSON *)0;
    cJSON *age = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *duplicate_person = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    double age_value = 0.0;
    int arr_count = 0;
    cJSON_bool replace_ok = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    name = cJSON_CreateString("Alice");
    age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(person, "age", age);
    cJSON_AddItemToObject(root, "person", person);
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate
    duplicate_person = cJSON_Duplicate(cJSON_GetObjectItem(root, "person"), 1);
    replacement = cJSON_CreateString("replaced");
    replace_ok = cJSON_ReplaceItemInObject(root, "person", replacement);
    detached = cJSON_DetachItemFromObject(root, "values");
    arr_count = cJSON_GetArraySize(detached);

    // step 4: Validate
    retrieved_name = cJSON_GetObjectItem(duplicate_person, "name");
    age_value = cJSON_GetNumberValue(cJSON_GetObjectItem(duplicate_person, "age"));
    cJSON_AddNumberToObject(root, "detached_values_count", (double)arr_count);
    cJSON_AddNumberToObject(root, "copied_age", age_value);

    // step 5: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(duplicate_person);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}