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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddNumberToObject(root, "pi", 3.141592653589793);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddNumberToObject(nested, "count", 7.0);

    // step 3: Operate and Validate
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    double scaled = pi_val * 10.0;
    cJSON_AddNumberToObject(root, "scaled_pi", scaled);
    cJSON *nested_item = cJSON_GetObjectItem(root, "nested");
    cJSON *count_item = cJSON_GetObjectItem(nested_item, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    double sum = scaled + count_val;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *compact = cJSON_PrintUnformatted(root);
    char *pretty = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(compact);
    cJSON_free(pretty);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}