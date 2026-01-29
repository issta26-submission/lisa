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
//<ID> 974
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = " { \"user\" : \"alice\" , \"active\" : false, \"meta\" : null } ";
    cJSON_Minify(json);
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *new_null = cJSON_CreateNull();
    cJSON_ReplaceItemInObject(root, "meta", new_null);
    cJSON_AddTrueToObject(root, "verified");

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, buffer, 512, 1);

    // step 4: Validate & Cleanup
    cJSON *user = cJSON_GetObjectItem(root, "user");
    char *user_name = cJSON_GetStringValue(user);
    (void)user_name;
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}