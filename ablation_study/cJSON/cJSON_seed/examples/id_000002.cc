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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example name");
    cJSON *nested = (cJSON*)0;
    cJSON *number_item = (cJSON*)0;
    cJSON_bool added_name = 0;

    // step 2: Configure
    added_name = cJSON_AddItemToObjectCS(root, "name", name_item);
    nested = cJSON_AddObjectToObject(root, "nested");

    // step 3: Operate and Validate
    number_item = cJSON_AddNumberToObject(nested, "value", 3.1415);
    cJSON_DeleteItemFromObject(root, "name");

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}