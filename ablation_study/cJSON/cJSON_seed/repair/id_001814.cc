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
//<ID> 1814
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *first = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("alpha");
    cJSON_SetValuestring(name, "alpha-updated");
    cJSON_AddItemToObject(first, "name", name);
    cJSON_AddNumberToObject(first, "value", 3.14);
    cJSON_AddItemToArray(items, first);

    // step 2: Configure
    const char *json_text = "{\"config\": {\"threshold\": 42, \"mode\": \"auto\"}} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 0);
    cJSON *cfg = cJSON_GetObjectItem(parsed, "config");
    cJSON_AddItemToObject(root, "config", cJSON_Duplicate(cfg, 1));
    cJSON_AddNumberToObject(root, "added_number", 7.0);
    cJSON_AddStringToObject(root, "cjson_version", cJSON_Version());

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int count = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *sname = cJSON_GetStringValue(cJSON_GetObjectItem(first_item, "name"));
    double num = cJSON_GetNumberValue(cJSON_GetObjectItem(first_item, "value"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = sname ? sname[0] : 'Z';
    buffer[2] = (char)('0' + (count % 10));
    buffer[3] = (char)('0' + (((int)num) % 10));
    (void)buffer; (void)sname; (void)num; (void)parse_end;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}