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
//<ID> 1247
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"value\":3.14159,\"flag\":true}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *val_item = NULL;
    cJSON *flag_item = NULL;
    cJSON *num_copy = NULL;
    char *printed_unformatted = NULL;
    char preallocated_buffer[256];
    cJSON_bool prealloc_result = 0;
    double extracted_value = 0.0;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    val_item = cJSON_GetObjectItem(root, "value");
    flag_item = cJSON_GetObjectItem(root, "flag");

    // step 3: Operate / Validate
    extracted_value = cJSON_GetNumberValue(val_item);
    is_true = cJSON_IsTrue(flag_item);
    printed_unformatted = cJSON_PrintUnformatted(root);
    prealloc_result = cJSON_PrintPreallocated(root, preallocated_buffer, (int)sizeof(preallocated_buffer), 0);
    num_copy = cJSON_CreateNumber(extracted_value);
    cJSON_AddItemToObject(root, "value_copy", num_copy);
    (void)parse_end;
    (void)prealloc_result;
    (void)is_true;

    // step 4: Cleanup
    cJSON_free(printed_unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}