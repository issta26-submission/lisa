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
//<ID> 1811
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"id\":1,\"name\":\"initial\",\"nested\":{\"value\":3.5}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    const char *version = cJSON_Version();

    // step 2: Configure
    cJSON_AddNumberToObject(root, "added", 42.0);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *new_name = cJSON_SetValuestring(name_item, "updated");
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddNumberToObject(extra, "e", 7.0);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    double added_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "added"));
    double nested_val = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "nested"), "value"));
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = version ? version[0] : 'V';
    buffer[2] = name_val ? name_val[0] : 'N';
    buffer[3] = (char)('0' + (((int)added_val) % 10));
    buffer[4] = (char)('0' + (((int)nested_val) % 10));
    (void)parse_end; (void)new_name; (void)buffer; (void)name_val; (void)added_val; (void)nested_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}