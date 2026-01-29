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
//<ID> 817
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *num_item = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddStringToObject(config, "name", "cjson_test");
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON_AddItemToObject(root, "pi", num_item);
    cJSON_SetNumberHelper(num_item, 3.14159);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    char *meta_pretty = cJSON_Print(meta);
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(meta_pretty);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}