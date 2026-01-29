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
//<ID> 885
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

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON *enabled = cJSON_AddTrueToObject(cfg, "enabled");

    // step 3: Operate
    cJSON_bool is_enabled = cJSON_IsTrue(enabled);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *dup = (char *)cJSON_malloc(printed_len + 1);
    memset(dup, 0, printed_len + 1);
    memcpy(dup, printed, printed_len);

    // step 4: Validate & Cleanup
    (void)is_enabled;
    cJSON_free(printed);
    cJSON_free(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}