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
//<ID> 1320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *title_item = cJSON_AddStringToObject(root, "title", "cJSON API Sequence");
    double numbers[3] = {3.14159, 2.71828, 1.41421};
    cJSON *array_item = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", array_item);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON *got_title = cJSON_GetObjectItem(root, "title");
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    cJSON_bool values_is_array = cJSON_IsArray(got_values);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (values_is_array ? 1 : 0));
    ((char *)scratch)[2] = got_title && got_title->valuestring ? got_title->valuestring[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}