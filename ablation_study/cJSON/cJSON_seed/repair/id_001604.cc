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
//<ID> 1604
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(flags, "enabled", enabled);
    cJSON *explicit_false = cJSON_CreateBool((cJSON_bool)0);
    cJSON_AddItemToObject(flags, "explicit_false", explicit_false);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", ver_item);
    cJSON *also_true = cJSON_CreateBool((cJSON_bool)1);
    cJSON_AddItemToObject(root, "also_true", also_true);

    // step 3: Operate and Validate
    char *rendered = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = rendered[0];
    buffer[1] = ver[0];

    // step 4: Cleanup
    cJSON_free(rendered);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}