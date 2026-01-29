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
//<ID> 1289
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
    cJSON *name = cJSON_CreateString("Charlie");
    cJSON *age = cJSON_CreateNumber(28.0);
    cJSON_AddItemToObject(user, "name", name);
    cJSON_AddItemToObject(user, "age", age);
    cJSON_AddItemToObject(root, "user", user);
    cJSON *scores = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateNumber(80.0);
    cJSON *s2 = cJSON_CreateNumber(92.5);
    cJSON_AddItemToArray(scores, s1);
    cJSON_AddItemToArray(scores, s2);
    cJSON_AddItemToObject(root, "scores", scores);
    cJSON *status = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "status", status);

    // step 2: Configure
    cJSON *replacement_status = cJSON_CreateString("active");
    cJSON_ReplaceItemViaPointer(root, status, replacement_status);
    cJSON *user_obj = cJSON_GetObjectItem(root, "user");
    cJSON *name_item = cJSON_GetObjectItem(user_obj, "name");

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    char *name_out = cJSON_PrintUnformatted(name_item);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = name_out[0];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(name_out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}