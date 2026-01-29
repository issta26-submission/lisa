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
//<ID> 1000
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Configure
    cJSON *config = cJSON_CreateObject();
    cJSON *dev_name = cJSON_CreateString("device-007");
    cJSON_AddItemToObject(config, "name", dev_name);
    cJSON_AddNumberToObject(config, "version", 1.5);
    cJSON_AddItemToObject(root, "config", config);

    // step 3: Operate & Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    int array_size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "computed_count", (double)array_size);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}