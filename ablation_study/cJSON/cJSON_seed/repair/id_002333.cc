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
//<ID> 2333
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
    cJSON *temp_num = cJSON_CreateNumber(123.45);
    cJSON_AddItemToObject(metrics, "temp", temp_num);

    // step 2: Configure
    cJSON *stats = cJSON_AddObjectToObject(root, "stats");
    cJSON *count_num = cJSON_CreateNumber(7.89);
    cJSON_AddItemToObject(stats, "count", count_num);

    // step 3: Operate and Validate
    cJSON *metrics_fetched = cJSON_GetObjectItem(root, "metrics");
    cJSON *temp_item = cJSON_GetObjectItem(metrics_fetched, "temp");
    cJSON_bool invalid = cJSON_IsInvalid(temp_item);
    char *json = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = invalid ? '1' : '0';
    cJSON_free(json);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}