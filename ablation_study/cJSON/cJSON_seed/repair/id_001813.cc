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
//<ID> 1813
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"initial\",\"count\":1}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *changed = cJSON_SetValuestring(name_item, "updated");
    cJSON_AddNumberToObject(root, "added", 42.0);
    (void)changed;

    // step 3: Operate and Validate
    const char *version = cJSON_Version();
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    int nchildren = cJSON_GetArraySize(root);
    buffer[0] = printed[0];
    buffer[1] = name_val[0];
    buffer[2] = (char)('0' + ((int)count_val % 10));
    buffer[3] = version[0];
    buffer[4] = (char)('0' + (nchildren % 10));
    (void)buffer; (void)name_val; (void)count_val; (void)version; (void)nchildren;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    return 66; // API sequence test completed successfully
}