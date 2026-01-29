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
//<ID> 779
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *active_item = NULL;
    cJSON *missing_item = NULL;
    char *printed = NULL;
    char *copied = NULL;
    size_t printed_len = 0;
    cJSON_bool is_false = 0;
    cJSON_bool is_null = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddFalseToObject(root, "active");
    cJSON_AddNullToObject(root, "missing");
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);

    // step 3: Operate / Validate
    copied = (char *)cJSON_malloc(printed_len + 1);
    memset(copied, 0, printed_len + 1);
    memcpy(copied, printed, printed_len + 1);
    active_item = cJSON_GetObjectItem(root, "active");
    missing_item = cJSON_GetObjectItem(root, "missing");
    is_false = cJSON_IsFalse(active_item);
    is_null = cJSON_IsNull(missing_item);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copied);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}