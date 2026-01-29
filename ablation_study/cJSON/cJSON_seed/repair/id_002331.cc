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
//<ID> 2331
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
    cJSON *num1 = cJSON_CreateNumber(123.456);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(metrics, "value1", num1);
    cJSON_AddItemToObject(metrics, "value2", num2);

    // step 3: Operate
    cJSON *fetched = cJSON_GetObjectItem(root, "metrics");
    cJSON *v1 = cJSON_GetObjectItem(fetched, "value1");
    cJSON_bool is_invalid = cJSON_IsInvalid(v1);
    char *json = cJSON_Print(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (char)(is_invalid ? '1' : '0');
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}