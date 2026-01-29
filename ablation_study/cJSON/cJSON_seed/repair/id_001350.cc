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
//<ID> 1350
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
    cJSON_AddNumberToObject(metrics, "value1", 10.5);
    cJSON_AddNumberToObject(metrics, "value2", 20.25);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "scale", 2.0);

    // step 3: Operate and Validate
    double before_v1 = cJSON_GetNumberValue(cJSON_GetObjectItem(metrics, "value1"));
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(metrics, "value1", replacement);
    double after_v1 = cJSON_GetNumberValue(cJSON_GetObjectItem(metrics, "value1"));
    double v2 = cJSON_GetNumberValue(cJSON_GetObjectItem(metrics, "value2"));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (replaced ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + ((int)before_v1 % 10));
    ((char *)scratch)[3] = (char)('0' + ((int)after_v1 % 10));
    ((char *)scratch)[4] = (char)('0' + ((int)v2 % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}