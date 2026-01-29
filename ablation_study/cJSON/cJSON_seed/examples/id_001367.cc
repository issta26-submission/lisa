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
//<ID> 1367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(3.5);
    cJSON *str = cJSON_CreateString("example");
    cJSON *nested = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "text", str);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddBoolToObject(root, "is_object", cJSON_IsObject(root));

    // step 3: Operate & Validate
    double orig = cJSON_GetNumberValue(num);
    double newval = cJSON_SetNumberHelper(num, orig + 1.5);
    cJSON *updated_item = cJSON_CreateNumber(newval);
    cJSON_AddItemToObject(root, "updated", updated_item);
    const char *txt = cJSON_GetStringValue(str);
    cJSON *copy = cJSON_CreateString(txt);
    cJSON_AddItemToObject(root, "text_copy", copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}