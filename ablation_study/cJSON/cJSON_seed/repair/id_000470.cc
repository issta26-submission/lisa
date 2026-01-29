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
//<ID> 470
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *raw_text = "{\"note\":\"raw data\",\"value\":123}";
    cJSON *raw_item = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_flag = cJSON_GetObjectItem(dup_root, "flag");
    cJSON_bool flag_was_false = cJSON_IsFalse(dup_flag);
    double flag_numeric = (double)flag_was_false;
    cJSON_AddNumberToObject(root, "flag_numeric", flag_numeric);

    // step 3: Operate
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}