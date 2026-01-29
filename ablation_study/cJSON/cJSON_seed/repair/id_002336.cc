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
//<ID> 2336
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
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");

    // step 2: Configure
    cJSON *threshold = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *score = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(metrics, "score", score);

    // step 3: Operate
    cJSON *cfg_fetched = cJSON_GetObjectItem(root, "config");
    cJSON *th_fetched = cJSON_GetObjectItem(cfg_fetched, "threshold");
    cJSON_bool invalid_flag = cJSON_IsInvalid(th_fetched);
    char *json = cJSON_Print(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = json ? json[0] : '\0';
    buffer[1] = (char)(int)cJSON_GetNumberValue(th_fetched);
    (void)invalid_flag;
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}