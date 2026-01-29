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
//<ID> 1615
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *null_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *num_item = NULL;
    char *out = NULL;
    cJSON_bool was_null = 0;
    double extracted_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    num_item = cJSON_AddNumberToObject(child, "count", 7.5);
    null_item = cJSON_CreateRaw("null");
    cJSON_AddItemToObject(root, "maybe", null_item);
    raw_item = cJSON_CreateRaw("{\"rawKey\":true}");
    cJSON_AddItemToObject(root, "payload", raw_item);

    // step 3: Operate / Validate
    out = cJSON_PrintUnformatted(root);
    was_null = cJSON_IsNull(cJSON_GetObjectItem(root, "maybe"));
    extracted_value = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "count"));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}