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
//<ID> 547
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
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "age", age);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(person, "active", active);

    // step 2: Configure
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *detached_name = cJSON_DetachItemFromObject(person, "name");
    cJSON_AddItemToArray(list, detached_name);
    cJSON *person_copy = cJSON_Duplicate(person, 1);
    cJSON_AddItemToArray(list, person_copy);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(list, 0);
    char *first_name = cJSON_GetStringValue(first);
    cJSON *reinserted = cJSON_CreateString(first_name);
    cJSON_AddItemToObject(root, "reinserted_name", reinserted);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}