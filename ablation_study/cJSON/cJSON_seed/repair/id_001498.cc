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
//<ID> 1498
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
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(root, "person", person);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(root, "age", age);
    cJSON *verified = cJSON_AddBoolToObject(root, "verified", 1);

    // step 2: Configure
    cJSON *person_ref = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person_ref, "name");
    char *old_name = cJSON_GetStringValue(name_item);
    char *set_ret = cJSON_SetValuestring(name_item, "Bob");
    char *new_name = cJSON_GetStringValue(name_item);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = old_name ? old_name[0] : 'x';
    scratch[1] = new_name ? new_name[0] : 'y';
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)((int)cJSON_GetNumberValue(cJSON_GetObjectItem(root, "age")) % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}