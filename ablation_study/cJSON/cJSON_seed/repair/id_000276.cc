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
//<ID> 276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"value\":21}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double val = cJSON_GetNumberValue(value_item);
    cJSON *doubled = cJSON_AddNumberToObject(root, "value_doubled", val * 2.0);
    cJSON *sum = cJSON_AddNumberToObject(root, "sum", val + (val * 2.0));

    // step 3: Operate and Validate
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}