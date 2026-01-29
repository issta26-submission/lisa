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
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *enabled = cJSON_AddTrueToObject(data, "enabled");
    cJSON *count = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(data, "count", count);
    void *mem = cJSON_malloc(64);
    memset(mem, 0, 64);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *data_copy = cJSON_Duplicate(data, 1);
    cJSON_AddItemToObject(root, "data_copy", data_copy);

    // step 3: Operate
    cJSON_bool is_enabled = cJSON_IsTrue(enabled);
    cJSON_bool add_ok = cJSON_AddItemToObject(root, "optional", cJSON_CreateNull());
    char *out = cJSON_PrintUnformatted(root);
    (void)is_enabled;
    (void)add_ok;

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(mem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}