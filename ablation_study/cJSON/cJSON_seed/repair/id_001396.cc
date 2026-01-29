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
//<ID> 1396
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 31415.0 / 10000.0);
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddNumberToObject(settings, "threshold", 42.0);

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddBoolToObject(settings, "enabled", 1);
    (void)enabled_item;
    cJSON *pi_item = cJSON_GetObjectItemCaseSensitive(root, "pi");

    // step 3: Operate and Validate
    double pi_val = cJSON_GetNumberValue(pi_item);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    (void)printed;
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = buf[0];
    scratch[1] = (char)('0' + ((int)pi_val % 10));
    scratch[2] = (char)('0' + (((int)(pi_val * 10.0)) % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}