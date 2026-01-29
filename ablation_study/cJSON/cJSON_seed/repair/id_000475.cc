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
//<ID> 475
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "raw_payload", raw);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_false);
    double flag_num = (double)flag_is_false;
    cJSON *num_item = cJSON_AddNumberToObject(root, "flag_numeric", flag_num);

    // step 3: Operate
    char *printed = cJSON_Print(root);
    char *dup_printed = cJSON_Print(dup_root);
    double retrieved = cJSON_GetNumberValue(num_item);
    cJSON_free(printed);
    cJSON_free(dup_printed);

    // step 4: Validate and Cleanup
    (void)retrieved;
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}