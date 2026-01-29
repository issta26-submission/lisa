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
//<ID> 1284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *name = "Alice";
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "person", person);
    cJSON *name_item = cJSON_CreateString(name);
    cJSON_AddItemToObject(person, "name", name_item);
    cJSON *age = cJSON_CreateNumber(30);
    cJSON_AddItemToObject(person, "age", age);
    cJSON *status_old = cJSON_CreateString("offline");
    cJSON_AddItemToObject(root, "status", status_old);

    // step 2: Configure
    cJSON *temp = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(temp, "flag", flag);

    // step 3: Operate and Validate
    cJSON *status_ptr = cJSON_GetObjectItem(root, "status");
    cJSON *status_new = cJSON_CreateString("online");
    cJSON_ReplaceItemViaPointer(root, status_ptr, status_new);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(temp);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}