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
//<ID> 1328
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *status_item = cJSON_AddStringToObject(root, "status", "ok");
    double numbers[3] = {1.5, 2.5, 3.5};
    cJSON *values_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", values_array);

    // step 2: Configure
    cJSON *retrieved_array = cJSON_GetObjectItem(root, "values");
    cJSON *retrieved_status = cJSON_GetObjectItem(root, "status");

    // step 3: Operate and Validate
    cJSON_bool is_array = cJSON_IsArray(retrieved_array);
    char *status_str = cJSON_GetStringValue(retrieved_status);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = status_str ? status_str[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (is_array ? 1 : 0));
    ((char *)scratch)[2] = out ? out[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}