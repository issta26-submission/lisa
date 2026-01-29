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
//<ID> 2339
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
    cJSON *year_num = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToObject(config, "year", year_num);
    cJSON *load_num = cJSON_CreateNumber(0.75);
    cJSON_AddItemToObject(metrics, "load", load_num);

    // step 3: Operate
    cJSON *fetched_config = cJSON_GetObjectItem(root, "config");
    cJSON *fetched_year = cJSON_GetObjectItem(fetched_config, "year");
    double year_value = cJSON_GetNumberValue(fetched_year);
    cJSON_bool year_invalid = cJSON_IsInvalid(fetched_year);
    char *json = cJSON_Print(root);

    // step 4: Validate and Cleanup
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (char)(int)year_value;
    buffer[2] = year_invalid ? '1' : '0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}