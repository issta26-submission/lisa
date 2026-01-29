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
//<ID> 1244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"flag\":true,\"value\":3.14}";
    cJSON *root = NULL;
    cJSON *flag = NULL;
    cJSON *value = NULL;
    char *printed_unformatted = NULL;
    char prealloc_buffer[128];
    cJSON_bool prealloc_ok = 0;
    cJSON_bool flag_is_true = 0;
    double original_value = 0.0;
    double updated_value = 0.0;
    memset(prealloc_buffer, 0, sizeof(prealloc_buffer));

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    flag = cJSON_GetObjectItem(root, "flag");
    value = cJSON_GetObjectItem(root, "value");

    // step 3: Operate / Validate
    flag_is_true = cJSON_IsTrue(flag);
    original_value = cJSON_GetNumberValue(value);
    cJSON_SetNumberHelper(value, original_value + 1.0);
    updated_value = cJSON_GetNumberValue(value);
    printed_unformatted = cJSON_PrintUnformatted(root);
    prealloc_ok = cJSON_PrintPreallocated(root, prealloc_buffer, (int)sizeof(prealloc_buffer), 0);

    // step 4: Cleanup
    cJSON_free(printed_unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}