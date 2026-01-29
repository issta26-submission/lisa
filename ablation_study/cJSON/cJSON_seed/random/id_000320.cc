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
//<ID> 320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    double value_read = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();

    // step 3: Configure
    num_item = cJSON_AddNumberToObject(root, "pi", 3.14159);
    str_item = cJSON_CreateString("example string");
    cJSON_AddItemToObject(root, "note", str_item);

    // step 4: Operate
    detached = cJSON_DetachItemViaPointer(root, str_item);
    is_arr = cJSON_IsArray(detached);
    value_read = cJSON_GetNumberValue(num_item);

    // step 5: Validate
    cJSON_AddNumberToObject(root, "read_back", value_read);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}