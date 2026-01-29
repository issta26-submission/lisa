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
//<ID> 888
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
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddNumberToObject(config, "initial", 10.0);

    // step 2: Configure
    cJSON *ref_item = cJSON_CreateString("sharedValue");
    cJSON_AddItemReferenceToObject(config, "shared", ref_item);

    // step 3: Operate and Validate
    cJSON *shared_ref = cJSON_GetObjectItem(config, "shared");
    char *shared_str = cJSON_GetStringValue(shared_ref);
    double derived = (double)shared_str[0];
    cJSON_AddNumberToObject(root, "derived_from_shared", derived);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}