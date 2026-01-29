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
//<ID> 1827
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *tag = cJSON_CreateString("release");
    cJSON_AddItemToObject(meta, "tag", tag);

    // step 3: Operate and Validate
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *nameItem = cJSON_GetObjectItem(root, "name");
    cJSON *valueItem = cJSON_GetObjectItem(root, "value");
    cJSON *addedItem = cJSON_GetObjectItem(root, "added");
    cJSON *tagItem = cJSON_GetObjectItem(meta, "tag");
    const char *name = cJSON_GetStringValue(nameItem);
    double value = cJSON_GetNumberValue(valueItem);
    double added = cJSON_GetNumberValue(addedItem);
    const char *tagstr = cJSON_GetStringValue(tagItem);
    buffer[0] = name ? name[0] : 'X';
    buffer[1] = (char)('0' + (((int)value) % 10));
    buffer[2] = (char)('0' + (((int)added) % 10));
    buffer[3] = ver ? ver[0] : 'V';
    buffer[4] = tagstr ? tagstr[0] : 'T';
    (void)parse_end; (void)name; (void)value; (void)added; (void)tagstr; (void)ver; (void)buffer;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}