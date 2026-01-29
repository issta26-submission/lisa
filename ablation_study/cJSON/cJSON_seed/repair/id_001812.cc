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
//<ID> 1812
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"initial\",\"count\":1}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *count_item = cJSON_GetObjectItem(root, "count");

    // step 2: Configure
    cJSON_SetValuestring(name_item, "renamed");
    cJSON_AddNumberToObject(root, "added", 42.5);
    const char *version = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    double added_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "added"));
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = version ? version[0] : 'Y';
    buffer[2] = (char)('0' + (((int)added_val) % 10));
    buffer[3] = name_val ? name_val[0] : 'Z';
    (void)buffer; (void)name_val; (void)added_val; (void)count_item;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}