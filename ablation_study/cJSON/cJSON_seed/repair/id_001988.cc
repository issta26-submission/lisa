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
//<ID> 1988
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
    cJSON *num_item = cJSON_CreateNumber(314.0 / 100.0);
    cJSON *name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "val", num_item);
    cJSON_AddItemToObject(config, "name", name_item);

    // step 2: Configure
    cJSON *config_ref = cJSON_CreateObjectReference(config);
    cJSON_AddItemToObject(root, "config_ref", config_ref);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", flag_false);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_config, "name");
    char *name_value = cJSON_GetStringValue(got_name);
    cJSON *got_val = cJSON_GetObjectItem(got_config, "val");
    double num_value = cJSON_GetNumberValue(got_val);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_false);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = name_value[0];
    buffer[1] = (char)('0' + ((int)num_value % 10));
    buffer[2] = (char)('0' + (flag_is_false % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}