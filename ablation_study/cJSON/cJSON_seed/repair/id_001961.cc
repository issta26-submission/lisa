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
//<ID> 1961
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
    cJSON *label = cJSON_CreateString("example_label");
    cJSON_AddItemToObject(config, "label", label);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "enabled", flag);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *ver = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObject(meta, "version", ver);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_label = cJSON_GetObjectItem(got_config, "label");
    cJSON *got_flag = cJSON_GetObjectItem(got_config, "enabled");
    const char *label_str = cJSON_GetStringValue(got_label);
    cJSON_bool flag_is_false = cJSON_IsFalse(got_flag);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 0);
    (void)printed;
    buffer[0] = (char)('0' + (flag_is_false % 10));
    buffer[1] = label_str ? label_str[0] : 'X';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}