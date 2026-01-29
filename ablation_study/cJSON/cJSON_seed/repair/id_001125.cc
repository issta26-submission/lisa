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
//<ID> 1125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.72);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "scale", 2.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *meta_lookup = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *scale_item = cJSON_GetObjectItemCaseSensitive(meta_lookup, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double a = cJSON_GetNumberValue(first);
    double b = cJSON_GetNumberValue(second);
    double computed_val = (a + b) * scale;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = out[0];
    scratch[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}