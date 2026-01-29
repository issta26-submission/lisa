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
//<ID> 1260
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *initial_bool = cJSON_CreateBool(1);
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *str = cJSON_CreateString("example_label");

    // step 2: Configure
    cJSON_AddItemToObject(root, "flag", initial_bool);
    cJSON_AddItemToObject(root, "count", num);
    cJSON_AddItemToObject(root, "label", str);
    cJSON *replacement_bool = cJSON_CreateBool(0);
    cJSON_ReplaceItemInObject(root, "flag", replacement_bool);

    // step 3: Operate & Validate
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_invalid = cJSON_IsInvalid(flag_item);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_value = cJSON_GetNumberValue(count_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)flag_is_invalid;
    (void)count_value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}