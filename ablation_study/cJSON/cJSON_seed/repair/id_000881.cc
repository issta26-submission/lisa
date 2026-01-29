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
//<ID> 881
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddNumberToObject(config, "threshold", 7.5);
    cJSON *name_item = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON *shared = cJSON_CreateObject();
    cJSON_AddNumberToObject(shared, "version", 2.0);
    cJSON_AddItemToObject(root, "shared_primary", shared);

    // step 2: Configure
    cJSON_AddItemReferenceToObject(config, "shared_ref", shared);
    cJSON_AddNumberToObject(root, "init_flag", 1.0);

    // step 3: Operate and Validate
    cJSON *name_lookup = cJSON_GetObjectItem(config, "name");
    char *name_str = cJSON_GetStringValue(name_lookup);
    double initial_code = (double)(unsigned char)name_str[0];
    cJSON_AddNumberToObject(root, "name_initial_code", initial_code);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}