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
//<ID> 879
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);
    const char *version = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(version);
    cJSON_AddItemToObjectCS(root, "version", ver_item);

    // step 2: Configure
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(cfg, "name", name_item);
    cJSON *opt_null = cJSON_CreateNull();
    cJSON_AddItemToObject(cfg, "optional", opt_null);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *retrieved_name_item = cJSON_GetObjectItem(cfg, "name");
    char *retrieved_name = cJSON_GetStringValue(retrieved_name_item);
    (void)retrieved_name;
    (void)version;

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}