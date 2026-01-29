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
//<ID> 61
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
    cJSON *name_item = cJSON_CreateString("initial_user");
    cJSON_AddItemToObject(root, "person", person);
    cJSON_AddItemToObject(person, "name", name_item);
    cJSON_AddNumberToObject(person, "age", 30.0);

    // step 2: Configure
    char *changed_name = cJSON_SetValuestring(name_item, "updated_user");
    (void)changed_name;
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddNumberToObject(metrics, "score", 85.5);

    // step 3: Operate and Validate
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON *age_item = cJSON_GetObjectItem(person, "age");
    double age_val = cJSON_GetNumberValue(age_item);
    double validation_val = age_val + (double)has_person;
    cJSON_AddNumberToObject(root, "validation", validation_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}