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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    cJSON *stats = cJSON_CreateObject();
    cJSON *count = cJSON_CreateNumber((double)cJSON_GetArraySize(values));
    cJSON_AddItemToObject(stats, "count", count);
    cJSON *average = cJSON_CreateNumber((cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2)) / 2.0);
    cJSON_AddItemToObject(stats, "average", average);
    cJSON_AddItemToObjectCS(root, "stats", stats);

    // step 3: Operate & Validate
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON_bool has_stats = cJSON_HasObjectItem(root, "stats");
    cJSON *present_sum = cJSON_CreateNumber((double)has_values + (double)has_stats);
    cJSON_AddItemToObject(root, "present_sum", present_sum);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}