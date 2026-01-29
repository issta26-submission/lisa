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
//<ID> 1326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *id_item = cJSON_AddStringToObject(root, "id", "node-1");
    double numbers[3] = {1.1, 2.2, 3.3};
    cJSON *values_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", values_array);

    // step 2: Configure
    cJSON *status_item = cJSON_AddStringToObject(root, "status", "active");
    char *out = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    cJSON *got_id = cJSON_GetObjectItem(root, "id");
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    cJSON_bool values_is_array = cJSON_IsArray(got_values);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (values_is_array ? 1 : 0));
    ((char *)scratch)[2] = (char)(got_id && got_id->valuestring ? got_id->valuestring[0] : 0);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}