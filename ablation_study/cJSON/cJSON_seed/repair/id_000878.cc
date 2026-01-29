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
//<ID> 878
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "name", "tester");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON *pi_node = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(pi_node, 123.456);
    cJSON_AddItemToObject(root, "pi_like", pi_node);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);

    // step 3: Operate and Validate
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi_like");
    double pi_value = cJSON_GetNumberValue(pi_item);
    cJSON *pi_copy = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(pi_copy, pi_value);
    cJSON_AddItemToObject(root, "pi_copy", pi_copy);
    cJSON *second_item = cJSON_GetArrayItem(values, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON_AddNumberToObject(root, "second_val", second_value);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}