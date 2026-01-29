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
//<ID> 1399
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
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");

    // step 2: Configure
    cJSON_AddNumberToObject(metrics, "value", 123.456);
    cJSON *ok_item = cJSON_AddBoolToObject(metrics, "ok", 1);
    cJSON_AddNumberToObject(meta, "id", 7.0);

    // step 3: Operate and Validate
    cJSON *value_item = cJSON_GetObjectItemCaseSensitive(metrics, "value");
    double value = cJSON_GetNumberValue(value_item);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *s = (char *)scratch;
    s[0] = buf[0];
    s[1] = ok_item ? '1' : '0';
    s[2] = (char)('0' + ((int)value % 10));
    s[3] = (char)('0' + (((int)value / 10) % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}