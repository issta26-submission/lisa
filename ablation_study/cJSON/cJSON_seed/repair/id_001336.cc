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
//<ID> 1336
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    cJSON *nested = cJSON_AddObjectToObject(meta, "nested");
    cJSON_AddItemToObject(nested, "initial", cJSON_CreateNumber(42.5));

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(values);
    cJSON *second_item = cJSON_GetArrayItem(values, 1);
    cJSON *replacement = cJSON_CreateNumber(20.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(values, second_item, replacement);
    cJSON *new_second = cJSON_GetArrayItem(values, 1);
    double new_val = cJSON_GetNumberValue(new_second);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (count % 10));
    ((char *)scratch)[2] = (char)('0' + (((int)new_val) % 10));
    ((char *)scratch)[3] = (char)('0' + (replaced ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}