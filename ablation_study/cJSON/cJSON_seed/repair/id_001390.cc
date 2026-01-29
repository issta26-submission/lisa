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
//<ID> 1390
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
    cJSON_AddNumberToObject(metrics, "count", 42.0);
    cJSON_AddNumberToObject(metrics, "ratio", 3.14);
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *metrics_obj = cJSON_GetObjectItemCaseSensitive(root, "metrics");
    cJSON *ratio_item = cJSON_GetObjectItemCaseSensitive(metrics_obj, "ratio");
    double ratio_val = cJSON_GetNumberValue(ratio_item);
    cJSON *count_item = cJSON_GetObjectItemCaseSensitive(metrics_obj, "count");
    double count_val = cJSON_GetNumberValue(count_item);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    char *s = (char *)scratch;
    s[0] = buf[0];
    s[1] = buf[1];
    s[2] = (char)('0' + ((int)count_val % 10));
    s[3] = (char)('0' + ((int)ratio_val % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}