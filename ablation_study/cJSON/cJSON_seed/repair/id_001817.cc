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
//<ID> 1817
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"name\":\"old\",\"value\":1}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(raw_json, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *new_name_ptr = cJSON_SetValuestring(name_item, "newname");
    const char *version = cJSON_Version();
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(version));
    cJSON_AddNumberToObject(root, "added", 42.5);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    double value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    double added = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "added"));
    const char *name_val = cJSON_GetStringValue(name_item);
    ((char*)buffer)[0] = printed ? printed[0] : 'X';
    ((char*)buffer)[1] = name_val ? name_val[0] : 'Y';
    ((char*)buffer)[2] = (char)('0' + ((int)value % 10));
    ((char*)buffer)[3] = (char)('0' + ((int)added % 10));
    (void)new_name_ptr; (void)parse_end; (void)version; (void)buffer; (void)name_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}