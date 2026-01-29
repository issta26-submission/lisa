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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObjectCS(meta, "version", cJSON_CreateNumber(42.0));
    cJSON_AddItemToObject(meta, "scale", cJSON_CreateNumber(2.5));

    // step 3: Operate & Validate
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    cJSON *summary = cJSON_CreateNumber((double)has_values + (double)has_meta);
    cJSON_AddItemToObject(root, "summary", summary);
    cJSON *pair_sum = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObjectCS(root, "pair_sum", pair_sum);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}