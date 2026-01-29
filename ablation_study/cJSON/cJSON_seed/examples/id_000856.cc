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
//<ID> 856
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
    cJSON *confirmed_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_bool added_to_array = cJSON_AddItemToArray(flags, true_item);
    cJSON_bool is_bool_flag = cJSON_IsBool(true_item);
    cJSON *bool_as_number = cJSON_CreateNumber((double)is_bool_flag);
    cJSON_bool added_number = cJSON_AddItemToArray(flags, bool_as_number);
    cJSON_bool added_flags = cJSON_AddItemToObject(root, "flags", flags);
    cJSON_bool added_confirmed = cJSON_AddItemToObject(root, "confirmed", confirmed_item);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON *retrieved_confirmed = cJSON_GetObjectItem(root, "confirmed");
    cJSON_bool confirmed_is_bool = cJSON_IsBool(retrieved_confirmed);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}