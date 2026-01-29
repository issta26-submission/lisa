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
//<ID> 2186
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
    cJSON_AddItemToObject(root, "values", values);
    cJSON *initial = cJSON_AddNumberToObject(root, "initial", 1.0);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(values, n2);
    cJSON *replacement = cJSON_CreateNumber(99.9);

    // step 3: Operate and Validate
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "initial", replacement);
    (void)replaced;
    cJSON_DeleteItemFromObjectCaseSensitive(root, "values");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}