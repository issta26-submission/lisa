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
//<ID> 479
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":false,\"count\":3}");
    cJSON_AddItemToObject(root, "raw_blob", raw);
    cJSON_AddNumberToObject(root, "top_count", 5.0);

    // step 2: Configure
    cJSON *raw_copy = cJSON_Duplicate(raw, 1);
    cJSON_AddItemToObject(root, "raw_copy", raw_copy);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);
    cJSON_bool was_false = cJSON_IsFalse(flag_false);
    cJSON_AddNumberToObject(root, "computed", 100.0 + (double)was_false);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON *computed_item = cJSON_GetObjectItem(root, "computed");
    double computed_value = cJSON_GetNumberValue(computed_item);
    (void)computed_value;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}