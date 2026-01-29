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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, flag_false);
    cJSON *base_num = cJSON_CreateNumber(10.5);
    cJSON_AddItemToObject(settings, "base", base_num);

    // step 2: Configure
    cJSON *first_flag = cJSON_GetArrayItem(flags, 0);
    cJSON *replacement_flag = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(flags, first_flag, replacement_flag);

    // step 3: Operate and Validate
    cJSON_bool is_bool = cJSON_IsBool(cJSON_GetArrayItem(flags, 0));
    double computed = (double)replaced + (double)is_bool + 3.14;
    cJSON_AddNumberToObject(settings, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}