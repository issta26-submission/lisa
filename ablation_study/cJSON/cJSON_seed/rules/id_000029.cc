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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.5);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *replacement = NULL;
    cJSON *retrieved = NULL;
    cJSON *validation_item = NULL;
    char *printed = NULL;
    double newval = 0.0;
    int validation = 0;
    cJSON_bool flagval = 0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "first", num1);
    cJSON_AddItemToObject(root, "second", num2);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    replacement = cJSON_CreateNumber(100.0);
    cJSON_ReplaceItemViaPointer(root, num1, replacement);
    retrieved = cJSON_GetObjectItem(root, "first");
    newval = cJSON_GetNumberValue(retrieved);
    flagval = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    validation = (int)newval + (int)flagval;
    validation_item = cJSON_CreateNumber((double)validation);
    cJSON_AddItemToObject(root, "validation", validation_item);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}