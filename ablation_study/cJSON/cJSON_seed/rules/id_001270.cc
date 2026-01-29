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
//<ID> 1270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *manual_num = cJSON_CreateNumber(7.0);
    cJSON *null_item = cJSON_CreateNull();
    cJSON *auto_get = NULL;
    cJSON *manual_get = NULL;
    cJSON *none_get = NULL;
    cJSON *status_get = NULL;
    const char *status_str = NULL;
    double auto_val = 0.0;
    double manual_val = 0.0;
    double sum_val = 0.0;
    cJSON *sum_item = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "manual", manual_num);
    cJSON_AddNumberToObject(root, "auto", 3.14);
    cJSON_AddItemToObject(root, "none", null_item);
    cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate / Validate
    auto_get = cJSON_GetObjectItem(root, "auto");
    manual_get = cJSON_GetObjectItem(root, "manual");
    none_get = cJSON_GetObjectItem(root, "none");
    status_get = cJSON_GetObjectItem(root, "status");
    status_str = cJSON_GetStringValue(status_get);
    auto_val = cJSON_GetNumberValue(auto_get);
    manual_val = cJSON_GetNumberValue(manual_get);
    sum_val = auto_val + manual_val;
    sum_item = cJSON_AddNumberToObject(root, "sum", sum_val);
    cJSON_AddStringToObject(root, "status_copy", status_str);

    // step 4: Cleanup
    (void)none_get;
    (void)sum_item;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}