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
//<ID> 1393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");
    cJSON_AddNumberToObject(metrics, "temperature", 23.5);
    cJSON_AddNumberToObject(metrics, "humidity", 55.0);
    cJSON *ok_item = cJSON_AddBoolToObject(metrics, "ok", 1);

    // step 2: Configure
    const cJSON *temp_item = cJSON_GetObjectItemCaseSensitive(metrics, "temperature");
    double temp_val = cJSON_GetNumberValue(temp_item);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *s = (char *)scratch;
    s[0] = buf[0];
    s[1] = (char)('0' + (int)printed);
    s[2] = (char)('0' + ((int)temp_val % 10));
    s[3] = (char)('0' + (int)(ok_item->valueint % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}