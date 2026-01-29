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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_AddObjectToObject(root, "person");
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddNumberToObject(person, "age", 30.0);

    // step 2: Configure
    char *replaced = cJSON_SetValuestring(name, "alice");
    (void)replaced;
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate and Validate
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON_bool has_name = cJSON_HasObjectItem(person, "name");
    cJSON *age_item = cJSON_GetObjectItem(person, "age");
    double age_val = cJSON_GetNumberValue(age_item);
    double computed = age_val + (double)has_person + (double)has_name;
    cJSON_AddNumberToObject(root, "age_next", computed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}