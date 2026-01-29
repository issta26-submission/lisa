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
//<ID> 1095
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
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(person, "age", age);
    cJSON_AddItemToObject(root, "person", person);

    // step 2: Configure
    cJSON *verified = cJSON_CreateFalse();
    cJSON_AddItemToObject(person, "verified", verified);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(person, "name"));
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 3: Operate & Validate
    cJSON *replacementName = cJSON_CreateString("Alice Smith");
    cJSON_ReplaceItemInObject(person, "name", replacementName);
    cJSON *verified_ptr = cJSON_GetObjectItem(person, "verified");
    cJSON *newVerified = cJSON_CreateTrue();
    cJSON_ReplaceItemViaPointer(person, verified_ptr, newVerified);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}