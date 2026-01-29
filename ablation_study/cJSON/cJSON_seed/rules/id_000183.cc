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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *double_item = NULL;
    const char *version = cJSON_Version();
    size_t version_len = 0;
    char *version_copy = NULL;
    double original_value = 0.0;
    cJSON_bool greeting_is_string = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Operate / Validate
    original_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    greeting_is_string = cJSON_IsString(cJSON_GetObjectItem(root, "greeting"));
    double_item = cJSON_CreateNumber(original_value * 2.0);
    cJSON_AddItemToObject(root, "double", double_item);
    version_len = (version != NULL) ? strlen(version) : 0;
    version_copy = (char *)cJSON_malloc(version_len + 1);
    if (version_copy) {
        memcpy(version_copy, version ? version : "", version_len + 1);
    }

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(version_copy);

    // API sequence test completed successfully
    (void)original_value;
    (void)greeting_is_string;
    (void)version;
    return 66;
}