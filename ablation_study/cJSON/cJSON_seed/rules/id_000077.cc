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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_text[] = "{\"name\":\"cJSON\",\"version\":1}";
    size_t buffer_length = sizeof(json_text) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, buffer_length, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *info = NULL;
    cJSON *name_item = NULL;
    cJSON *version_item = NULL;
    const char *name_str = NULL;
    double version_val = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "info", parsed);
    cJSON_AddNumberToObject(root, "extra", 3.14);

    // step 3: Operate / Validate
    info = cJSON_GetObjectItem(root, "info");
    name_item = cJSON_GetObjectItem(info, "name");
    version_item = cJSON_GetObjectItem(info, "version");
    name_str = cJSON_GetStringValue(name_item);
    version_val = cJSON_GetNumberValue(version_item);
    cJSON_AddNumberToObject(root, "sum", version_val + 3.14);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}