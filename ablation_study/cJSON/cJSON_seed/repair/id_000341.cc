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
//<ID> 341
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
    cJSON *init_num = cJSON_CreateNumber(10.5);
    cJSON_AddItemToObject(metrics, "initial", init_num);
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *label = cJSON_CreateString("v1.2");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    const char *label_str = cJSON_GetStringValue(label);
    double label_code = (double)label_str[0];
    cJSON *computed = cJSON_CreateNumber(label_code + 2.0);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Operate and Validate
    cJSON_bool is_true = cJSON_IsTrue(enabled);
    cJSON *is_true_num = cJSON_CreateNumber((double)is_true);
    cJSON_AddItemToObject(root, "is_true_num", is_true_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}