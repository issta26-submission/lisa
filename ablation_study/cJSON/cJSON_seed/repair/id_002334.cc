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
//<ID> 2334
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

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(metrics, "value", num);
    cJSON *unit = cJSON_CreateString("ms");
    cJSON_AddItemToObject(metrics, "unit", unit);

    // step 3: Operate
    cJSON *metrics_fetched = cJSON_GetObjectItem(root, "metrics");
    cJSON *value_item = cJSON_GetObjectItem(metrics_fetched, "value");
    cJSON_bool invalid_flag = cJSON_IsInvalid(value_item);
    char *json = cJSON_Print(root);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (char)(invalid_flag ? 1 : 0);
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}