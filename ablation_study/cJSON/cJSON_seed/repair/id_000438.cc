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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"enabled\":true},\"data\":{\"x\":42,\"y\":3.14}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *data = cJSON_GetObjectItem(root, "data");

    // step 2: Configure
    cJSON *added = cJSON_AddObjectToObject(root, "added");
    cJSON *data_ref = cJSON_CreateObjectReference(data);
    cJSON_AddItemToObject(added, "data_ref", data_ref);

    // step 3: Operate
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    char *serialized = cJSON_Print(root);
    (void)has_config;

    // step 4: Validate and Cleanup
    cJSON_free(serialized);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}