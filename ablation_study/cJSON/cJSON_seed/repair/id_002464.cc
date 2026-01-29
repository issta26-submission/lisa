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
//<ID> 2464
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
    cJSON *enabled_item = cJSON_AddTrueToObject(config, "enabled");
    cJSON *broken_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "broken", broken_item);
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(root, "version", cJSON_CreateNumber(1.0));

    // step 2: Configure
    char *printed = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON_bool broken_is_false = cJSON_IsFalse(broken_item);
    buffer[1] = (char)(broken_is_false ? 'F' : 'N');
    cJSON *cfg_ref = cJSON_GetObjectItem(root, "config");
    cJSON *enabled_ref = cJSON_GetObjectItem(cfg_ref, "enabled");
    cJSON_bool enabled_is_false = cJSON_IsFalse(enabled_ref);
    buffer[2] = (char)(enabled_is_false ? 't' : 'T');

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}