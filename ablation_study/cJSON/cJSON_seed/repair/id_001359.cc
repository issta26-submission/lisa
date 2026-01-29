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
//<ID> 1359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");

    // step 2: Configure
    cJSON *timeout = cJSON_AddNumberToObject(settings, "timeout", 30.0);
    cJSON *retries = cJSON_AddNumberToObject(settings, "retries", 3.0);
    cJSON *new_timeout = cJSON_CreateNumber(60.0);

    // step 3: Operate and Validate
    cJSON_bool replaced = cJSON_ReplaceItemInObject(settings, "timeout", new_timeout);
    double val_timeout = cJSON_GetNumberValue(cJSON_GetObjectItem(settings, "timeout"));
    double val_retries = cJSON_GetNumberValue(retries);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out[0];
    buf[1] = (char)('0' + (int)replaced);
    buf[2] = (char)('0' + ((int)val_timeout % 10));
    buf[3] = (char)('0' + ((int)val_retries % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}