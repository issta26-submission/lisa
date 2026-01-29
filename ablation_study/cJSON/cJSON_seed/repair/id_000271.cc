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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"value\":10}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *added = cJSON_AddNumberToObject(root, "added_number", 3.1415);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate and Validate
    cJSON *orig_item = cJSON_GetObjectItem(root, "value");
    double orig_val = cJSON_GetNumberValue(orig_item);
    cJSON *copy = cJSON_AddNumberToObject(root, "value_copy", orig_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}