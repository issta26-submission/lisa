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
//<ID> 1188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *orig_value = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *standalone = NULL;
    double new_value = 0.0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "value", num1);
    cJSON_AddTrueToObject(root, "flag");
    standalone = cJSON_CreateString("orphan");

    // step 3: Operate / Validate
    orig_value = cJSON_GetObjectItem(root, "value");
    num2 = cJSON_CreateNumber(456.0);
    replaced = cJSON_ReplaceItemViaPointer(root, orig_value, num2);
    new_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    (void)replaced;
    (void)new_value;

    // step 4: Cleanup
    cJSON_Delete(standalone);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}