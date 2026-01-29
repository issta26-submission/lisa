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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"orig\":7}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "added", 99.5);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate and Validate
    cJSON *orig_item = cJSON_GetObjectItem(root, "orig");
    double orig_val = cJSON_GetNumberValue(orig_item);
    cJSON_AddNumberToObject(root, "orig_copy", orig_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}