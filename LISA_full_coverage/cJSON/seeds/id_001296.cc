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
//<ID> 1296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\":{\"value\":42},\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "parsed", parsed);
    cJSON *config = cJSON_GetObjectItem(root, "parsed");
    cJSON *value_item = cJSON_GetObjectItem(config, "value");
    cJSON *new_number = cJSON_CreateNumber(100.5);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(config, value_item, new_number);
    (void)replaced;
    cJSON *extra = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObjectCS(root, "extra", extra);

    // step 3: Operate
    cJSON *got_config = cJSON_GetObjectItem(root, "parsed");
    cJSON *got_value = cJSON_GetObjectItem(got_config, "value");
    double val = cJSON_GetNumberValue(got_value);
    (void)val;

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}