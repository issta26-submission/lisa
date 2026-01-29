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
//<ID> 1273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *temp_num = cJSON_CreateNumber(7.0);
    cJSON *pi_node = NULL;
    cJSON *status_node = NULL;
    cJSON *missing_node = NULL;
    cJSON *retrieved = NULL;
    char *status_str = NULL;
    double pi_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "temp", temp_num);
    pi_node = cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddStringToObject(root, "status", "ok");
    missing_node = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", missing_node);

    // step 3: Operate / Validate
    status_node = cJSON_GetObjectItem(root, "status");
    status_str = cJSON_GetStringValue(status_node);
    retrieved = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(retrieved);
    (void)status_str;
    (void)pi_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}