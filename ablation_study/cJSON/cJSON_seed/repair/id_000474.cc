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
//<ID> 474
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
    cJSON_AddItemToObject(root, "raw_embedded", raw);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 2: Configure
    cJSON_bool flag_false = cJSON_IsFalse(false_item);
    cJSON_AddNumberToObject(root, "flag_numeric", (double)flag_false);
    cJSON *dup_raw = cJSON_Duplicate(raw, 1);
    cJSON_AddItemToObject(root, "raw_duplicate", dup_raw);
    cJSON_AddNumberToObject(root, "count", 3.14);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}