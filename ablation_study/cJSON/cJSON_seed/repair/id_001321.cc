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
//<ID> 1321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *title_item = cJSON_AddStringToObject(root, "title", "SensorReadings");
    double numbers[3] = {1.1, 2.2, 3.3};
    cJSON *values = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    int values_count = cJSON_GetArraySize(values);

    // step 3: Operate and Validate
    cJSON *values_ptr = cJSON_GetObjectItem(root, "values");
    cJSON *title_ptr = cJSON_GetObjectItem(root, "title");
    cJSON_bool values_is_array = cJSON_IsArray(values_ptr);
    const char *title_str = cJSON_GetStringValue(title_ptr);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = (char)('0' + (values_is_array ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (values_count % 10));
    ((char *)scratch)[3] = title_str[0];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}