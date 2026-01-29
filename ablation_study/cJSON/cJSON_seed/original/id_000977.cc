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
//<ID> 977
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = " { \"alpha\": 123, \"beta\": false, \"gamma\": \"hello\" } ";
    cJSON_Minify(json);
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", null_item);
    cJSON *gamma = cJSON_GetObjectItem(root, "gamma");
    cJSON_SetValuestring(gamma, "hello_world");

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "alpha");
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}