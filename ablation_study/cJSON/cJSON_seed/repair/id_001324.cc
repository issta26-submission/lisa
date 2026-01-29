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
//<ID> 1324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *title_item = cJSON_AddStringToObject(root, "title", "FuzzTest");
    double values_arr_data[3] = {1.1, 2.2, 3.3};
    cJSON *values_array = cJSON_CreateDoubleArray(values_arr_data, 3);
    cJSON_AddItemToObject(root, "values", values_array);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "lang", "en");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    cJSON_bool values_is_array = cJSON_IsArray(got_values);
    cJSON *got_title = cJSON_GetObjectItem(root, "title");
    const char *title_str = cJSON_GetStringValue(got_title);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = out ? out[0] : 0;
    scratch[1] = (char)('0' + (values_is_array ? 1 : 0));
    scratch[2] = title_str ? title_str[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}