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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *ver_item = NULL;
    void *buffer = cJSON_malloc((size_t)128);
    memset(buffer, 0, 128);
    double extracted_number = 0.0;
    cJSON_bool greeting_is_string = 0;
    const char *version_str = cJSON_Version();

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "value", num_item);
    cJSON_AddItemToObject(root, "greeting", str_item);
    ver_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "version", ver_item);

    // step 3: Operate / Validate
    extracted_number = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "value"));
    greeting_is_string = cJSON_IsString(cJSON_GetObjectItem(root, "greeting"));
    (void)extracted_number;
    (void)greeting_is_string;
    (void)version_str;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}