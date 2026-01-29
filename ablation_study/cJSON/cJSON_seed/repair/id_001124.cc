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
//<ID> 1124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "values", arr);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON *n2 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *mult = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "multiplier", mult);

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "computed_sum_times_multiplier");
    cJSON *meta = cJSON_CreateString("meta");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *got_data = cJSON_GetObjectItemCaseSensitive(root, "data");
    cJSON *arr_from_data = cJSON_GetObjectItemCaseSensitive(got_data, "values");
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr_from_data, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr_from_data, 1));
    double multiplier = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(root, "multiplier"));
    double computed = (v0 + v1) * multiplier;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(32);
    memset(scratch, 0, 32);
    scratch[0] = out ? out[0] : '\0';
    scratch[1] = out ? out[1] : '\0';
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}