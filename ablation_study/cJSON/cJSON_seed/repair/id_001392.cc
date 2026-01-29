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
//<ID> 1392
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_AddObjectToObject(root, "payload");
    cJSON_AddNumberToObject(payload, "temperature", 36.5);
    cJSON_AddNumberToObject(payload, "pressure", 101.3);
    cJSON *active_item = cJSON_AddBoolToObject(payload, "active", 1);

    // step 2: Configure
    const cJSON *temp_item = cJSON_GetObjectItemCaseSensitive(payload, "temperature");
    const cJSON *active_check = cJSON_GetObjectItemCaseSensitive(payload, "active");

    // step 3: Operate and Validate
    double temp_val = cJSON_GetNumberValue(temp_item);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    void *scratch_alloc = cJSON_malloc(16);
    memset(scratch_alloc, 0, 16);
    char *scratch = (char *)scratch_alloc;
    scratch[0] = buf[0];
    scratch[1] = buf[1];
    scratch[2] = (char)('0' + (int)printed);
    scratch[3] = (char)('0' + ((int)temp_val % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch_alloc);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}