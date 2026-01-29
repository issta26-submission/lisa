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
//<ID> 1183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *ok_item = NULL;
    cJSON *replacement = NULL;
    cJSON *value_item = NULL;
    cJSON *temp = NULL;
    double value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "value", 42.0);
    cJSON_AddStringToObject(root, "name", "original");
    cJSON_AddTrueToObject(root, "ok");
    temp = cJSON_CreateNumber(3.14);

    // step 3: Operate / Validate
    ok_item = cJSON_GetObjectItem(root, "ok");
    replacement = cJSON_CreateFalse();
    cJSON_ReplaceItemViaPointer(root, ok_item, replacement);
    value_item = cJSON_GetObjectItem(root, "value");
    value = cJSON_GetNumberValue(value_item);
    (void)value;

    // step 4: Cleanup
    cJSON_Delete(temp);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}