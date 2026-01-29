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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations & Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("original_name");
    cJSON *value_item = cJSON_CreateNumber(21.0);
    cJSON *replacement_false = cJSON_CreateFalse();

    // step 2: Configure - attach items to the root object
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "value", value_item);

    // step 3: Operate - replace the "name" item (via pointer) with a false boolean
    cJSON_ReplaceItemViaPointer(root, name_item, replacement_false);

    // step 4: Validate - read numeric value and store a derived value back into the object
    cJSON *retrieved_value = cJSON_GetObjectItem(root, "value");
    double num = cJSON_GetNumberValue(retrieved_value);
    cJSON_AddNumberToObject(root, "value_times_two", num * 2.0);
    cJSON_AddStringToObject(root, "status", "replaced");

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}