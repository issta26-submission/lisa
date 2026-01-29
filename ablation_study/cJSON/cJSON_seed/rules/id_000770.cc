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
//<ID> 770
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const size_t scratch_len = 256;
    char *scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    char *printed = NULL;
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *enabled_item = NULL;
    cJSON *optional_item = NULL;
    cJSON *count_item = NULL;
    const char *child_desc = NULL;
    double count_value = 0.0;
    int enabled_is_false = 0;
    int optional_is_null = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddFalseToObject(root, "enabled");
    cJSON_AddNullToObject(root, "optional");
    cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON_AddStringToObject(child, "desc", "inner description");

    // step 3: Operate / Validate
    enabled_item = cJSON_GetObjectItem(root, "enabled");
    optional_item = cJSON_GetObjectItem(root, "optional");
    count_item = cJSON_GetObjectItem(root, "count");
    count_value = cJSON_GetNumberValue(count_item);
    enabled_is_false = cJSON_IsFalse(enabled_item);
    optional_is_null = cJSON_IsNull(optional_item);
    child_desc = cJSON_GetStringValue(cJSON_GetObjectItem(child, "desc"));
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < (scratch_len - 1) ? printed_len : (scratch_len - 1);
    if (printed) {
        memcpy(scratch, printed, to_copy);
        scratch[to_copy] = '\0';
    }

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}