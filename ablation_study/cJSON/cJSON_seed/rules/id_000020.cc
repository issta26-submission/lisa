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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *num_orig = cJSON_CreateNumber(42.0);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *replacement = NULL;
    cJSON *validation_item = NULL;
    char *printed = NULL;
    cJSON_bool replaced = 0;
    double computed = 0.0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "value", num_orig);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate/Validate
    replacement = cJSON_CreateNumber(99.0);
    replaced = cJSON_ReplaceItemViaPointer(root, num_orig, replacement);
    computed = cJSON_GetNumberValue(replacement) * (double)cJSON_IsTrue(flag);
    validation_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "validation", validation_item);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}