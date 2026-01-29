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
//<ID> 1244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *person_name = "Alice";
    double person_age = 30.0;
    cJSON *person = cJSON_CreateObject();
    cJSON_AddStringToObject(person, "name", person_name);
    cJSON_AddNumberToObject(person, "age", person_age);

    // step 2: Configure
    cJSON *person_dup = cJSON_Duplicate(person, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "original", person);
    cJSON_AddItemToObject(root, "copy", person_dup);
    cJSON_bool original_is_array = cJSON_IsArray(person);
    cJSON_AddBoolToObject(root, "original_is_array", original_is_array);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}