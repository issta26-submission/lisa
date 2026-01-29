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
//<ID> 1810
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *nameItem = cJSON_CreateString("placeholder");
    cJSON_AddItemToObject(meta, "name", nameItem);
    cJSON_SetValuestring(nameItem, version);
    cJSON *initialNum = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "initial", initialNum);

    // step 3: Operate and Validate
    const char *json = "{\"external\": 123.5}";
    const char *parse_end = 0;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    double external_val = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "external"));
    cJSON_AddNumberToObject(root, "external_copy", external_val);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    double copied_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "external_copy"));
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (((int)copied_val) % 10));
    (void)buffer; (void)copied_val; (void)external_val; (void)version;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}