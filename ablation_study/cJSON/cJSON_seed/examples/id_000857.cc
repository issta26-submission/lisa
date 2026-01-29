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
//<ID> 857
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
    cJSON *t1 = cJSON_CreateTrue();
    cJSON *t2 = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(flags, t1);
    cJSON_AddItemToArray(flags, t2);
    cJSON_AddItemToObject(root, "flags", flags);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first);
    cJSON *first_flag_numeric = cJSON_CreateNumber((double)first_is_bool);
    cJSON_AddItemToObject(root, "first_is_bool", first_flag_numeric);
    char *snapshot = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}