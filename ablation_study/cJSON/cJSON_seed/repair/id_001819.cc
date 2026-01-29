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
//<ID> 1819
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"beta\",\"value\":42}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON_AddNumberToObject(root, "added", 123.456);
    cJSON *nameItem = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(nameItem, "gamma");

    // step 3: Operate and Validate
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const char *name = cJSON_GetStringValue(nameItem);
    double value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    double added = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "added"));
    buffer[0] = name ? name[0] : 'X';
    buffer[1] = (char)('0' + (((int)value) % 10));
    buffer[2] = (char)('0' + (((int)added) % 10));
    buffer[3] = ver ? ver[0] : 'V';
    (void)buffer; (void)parse_end; (void)name; (void)value; (void)added; (void)ver;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}