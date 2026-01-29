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
//<ID> 1612
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *null_item = NULL;
    char *out = NULL;
    double extracted = 0.0;
    cJSON_bool was_null = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "payload", raw_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", null_item);

    // step 3: Operate / Validate
    was_null = cJSON_IsNull(cJSON_GetObjectItem(root, "maybe"));
    extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}