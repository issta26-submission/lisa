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
//<ID> 273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\":3.5,\"b\":2.5}";
    cJSON *root = cJSON_Parse(json);
    cJSON *a_item = cJSON_GetObjectItem(root, "a");
    double a_val = cJSON_GetNumberValue(a_item);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);
    cJSON_AddNumberToObject(root, "a_copy", a_val);

    // step 3: Operate and Validate
    cJSON *b_item = cJSON_GetObjectItem(root, "b");
    double b_val = cJSON_GetNumberValue(b_item);
    cJSON_AddNumberToObject(root, "sum", a_val + b_val);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}