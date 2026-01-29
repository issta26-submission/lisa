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
//<ID> 859
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(flags, true_item);
    cJSON_AddItemToObject(root, "flags", flags);

    // step 3: Operate & Validate
    cJSON *first_elem = cJSON_GetArrayItem(flags, 0);
    cJSON_bool is_bool = cJSON_IsBool(first_elem);
    cJSON *bool_number = cJSON_CreateNumber((double)is_bool);
    cJSON_AddItemToObject(root, "bool_as_number", bool_number);
    char *snapshot = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}