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
//<ID> 1367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *original = NULL;
    cJSON *raw = NULL;
    cJSON *field_item = NULL;
    const char *retrieved_str = NULL;
    double retrieved_num = 0.0;
    cJSON_bool replaced = 0;
    cJSON_bool is_raw = 0;
    char *buffer = NULL;
    int buf_len = 512;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    original = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "field", original);
    cJSON_AddNumberToObject(root, "count", 7.0);

    // step 3: Operate / Validate
    raw = cJSON_CreateRaw("raw_payload");
    replaced = cJSON_ReplaceItemInObject(root, "field", raw);
    field_item = cJSON_GetObjectItem(root, "field");
    is_raw = cJSON_IsRaw(field_item);
    retrieved_num = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}