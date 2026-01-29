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
//<ID> 1363
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
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *scores = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateNumber(85.0);
    cJSON *s2 = cJSON_CreateNumber(90.5);

    // step 2: Configure
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(person, "age", age);
    cJSON_AddItemToArray(scores, s1);
    cJSON_AddItemToArray(scores, s2);
    cJSON_AddItemToObject(root, "person", person);
    cJSON_AddItemToObject(root, "scores", scores);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddBoolToObject(root, "person_is_object", cJSON_IsObject(person));

    // step 3: Operate & Validate
    char *name_val = cJSON_GetStringValue(name);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    double age_val = cJSON_GetNumberValue(age);
    double new_age = age_val + 1.0;
    double set_ret = cJSON_SetNumberHelper(age, new_age);
    double age_after = cJSON_GetNumberValue(age);
    double sc1 = cJSON_GetNumberValue(s1);
    double sc2 = cJSON_GetNumberValue(s2);
    double total = sc1 + sc2 + age_after;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_item);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *printed_str = cJSON_CreateString(printed);
    cJSON_AddItemToObject(root, "printed", printed_str);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    // API sequence test completed successfully
    return 66;
}