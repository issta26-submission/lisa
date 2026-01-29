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
//<ID> 1982
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
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "count", count);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", enabled);

    // step 2: Configure
    cJSON *config_ref = cJSON_CreateObjectReference(config);
    cJSON_AddItemToObject(root, "config_ref", config_ref);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_count = cJSON_GetObjectItem(got_config, "count");
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    cJSON *got_enabled = cJSON_GetObjectItem(got_config, "enabled");
    double number_val = cJSON_GetNumberValue(got_count);
    const char *name_val = cJSON_GetStringValue(got_name);
    cJSON_bool flag_false = cJSON_IsFalse(got_enabled);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('0' + (int)number_val % 10);
    buffer[1] = '|';
    buffer[2] = (char)('0' + (flag_false % 10));
    buffer[3] = name_val ? name_val[0] : '\0';
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}