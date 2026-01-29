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
//<ID> 1246
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"value\":42.5,\"flag\":true}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = NULL;
    cJSON *val_item = NULL;
    cJSON *flag_item = NULL;
    char *printed_unformatted = NULL;
    char prealloc_buffer[128];
    memset(prealloc_buffer, 0, sizeof(prealloc_buffer));
    cJSON_bool prealloc_result = 0;
    double extracted_value = 0.0;
    cJSON_bool flag_is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLength(json, json_len);
    val_item = cJSON_GetObjectItem(root, "value");
    flag_item = cJSON_GetObjectItem(root, "flag");

    // step 3: Operate / Validate
    extracted_value = cJSON_GetNumberValue(val_item);
    flag_is_true = cJSON_IsTrue(flag_item);
    printed_unformatted = cJSON_PrintUnformatted(root);
    prealloc_result = cJSON_PrintPreallocated(root, prealloc_buffer, (int)sizeof(prealloc_buffer), 0);
    cJSON_AddNumberToObject(root, "value_copy", extracted_value);
    cJSON_free(printed_unformatted);
    (void)prealloc_result;
    (void)flag_is_true;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}