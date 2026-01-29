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
//<ID> 1815
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"name\":\"original\",\"count\":7}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(raw_json, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "updated");
    cJSON_AddNumberToObject(root, "added", 42.0);
    const char *version = cJSON_Version();

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const char *name_str = cJSON_GetStringValue(name_item);
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    double added_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "added"));
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Y';
    buffer[2] = (char)('0' + (((int)count_val) % 10));
    buffer[3] = (char)('0' + (((int)added_val) % 10));
    buffer[4] = version ? version[0] : 'V';
    (void)buffer; (void)parse_end; (void)version;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}