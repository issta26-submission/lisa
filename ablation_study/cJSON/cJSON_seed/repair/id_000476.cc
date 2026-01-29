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
//<ID> 476
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":123}");
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON *num_item = cJSON_AddNumberToObject(root, "original", 42.5);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 2: Configure
    cJSON *dup_raw = cJSON_Duplicate(raw, 1);
    cJSON_AddItemToObject(root, "raw_copy", dup_raw);
    cJSON_bool was_false = cJSON_IsFalse(false_item);
    double orig_value = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "copy", orig_value);
    (void)was_false;

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}