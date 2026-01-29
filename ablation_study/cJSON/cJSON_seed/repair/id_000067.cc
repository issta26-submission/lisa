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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *user = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "user", user);
    cJSON *name = cJSON_CreateString("initial");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(user, "name", name);
    cJSON_AddItemToObject(user, "age", age);

    // step 2: Configure
    char *set_ptr = cJSON_SetValuestring(name, "tester42");
    (void)set_ptr;
    cJSON_AddNumberToObject(user, "id", 1001.0);

    // step 3: Operate and Validate
    cJSON_bool has_user = cJSON_HasObjectItem(root, "user");
    cJSON_bool has_email = cJSON_HasObjectItem(user, "email");
    cJSON_AddNumberToObject(root, "has_user", (double)has_user);
    cJSON_AddNumberToObject(root, "has_email", (double)has_email);
    cJSON *age_item = cJSON_GetObjectItem(user, "age");
    double age_val = cJSON_GetNumberValue(age_item);
    cJSON_AddNumberToObject(root, "age_plus_one", age_val + 1.0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}