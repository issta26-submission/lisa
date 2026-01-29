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
//<ID> 1327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = { 1.1, 2.2, 3.3 };
    cJSON *values_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "cJSON_Test");
    cJSON_AddItemToObject(root, "values", values_array);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "creator", "assistant");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *fetched_name = cJSON_GetObjectItem(root, "name");
    cJSON *fetched_values = cJSON_GetObjectItem(root, "values");
    cJSON_bool values_is_array = cJSON_IsArray(fetched_values);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (values_is_array ? 1 : 0));
    cJSON *meta_obj = cJSON_GetObjectItem(root, "meta");
    cJSON *creator_item = cJSON_GetObjectItem(meta_obj, "creator");
    char *creator_str = cJSON_GetStringValue(creator_item);
    ((char *)scratch)[2] = creator_str ? creator_str[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}