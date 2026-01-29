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
//<ID> 1240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"alpha\",\"value\":3.14,\"flag\":true}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *name = NULL;
    cJSON *value = NULL;
    cJSON *flag = NULL;
    char *printed_unformatted = NULL;
    char buffer[256];
    char *printed_prealloc_ptr = NULL;
    double numeric_value = 0.0;
    cJSON_bool flag_is_true = 0;
    cJSON_bool prealloc_result = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    name = cJSON_GetObjectItem(root, "name");
    value = cJSON_GetObjectItem(root, "value");
    flag = cJSON_GetObjectItem(root, "flag");

    // step 3: Operate / Validate
    numeric_value = cJSON_GetNumberValue(value);
    flag_is_true = cJSON_IsTrue(flag);
    printed_unformatted = cJSON_PrintUnformatted(root);
    memset(buffer, 0, sizeof(buffer));
    prealloc_result = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    printed_prealloc_ptr = buffer;
    (void)name;
    (void)parse_end;
    (void)printed_prealloc_ptr;
    (void)numeric_value;
    (void)flag_is_true;
    (void)prealloc_result;
    (void)json_len;

    // step 4: Cleanup
    cJSON_free(printed_unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}