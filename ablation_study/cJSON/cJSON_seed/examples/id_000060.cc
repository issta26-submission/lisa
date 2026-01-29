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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "metrics", metrics);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(12.5);
    cJSON *n2 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(metrics, "a", n1);
    cJSON_AddItemToObjectCS(metrics, "b", n2);

    // step 3: Operate & Validate
    cJSON *got_a = cJSON_GetObjectItem(metrics, "a");
    cJSON *got_b = cJSON_GetObjectItem(metrics, "b");
    double va = cJSON_GetNumberValue(got_a);
    double vb = cJSON_GetNumberValue(got_b);
    cJSON *sum_item = cJSON_CreateNumber(va + vb);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_bool has_metrics = cJSON_HasObjectItem(root, "metrics");
    cJSON_bool has_sum = cJSON_HasObjectItem(root, "sum");
    cJSON *status = cJSON_CreateNumber((double)has_metrics + (double)has_sum);
    cJSON_AddItemToObjectCS(root, "status", status);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}