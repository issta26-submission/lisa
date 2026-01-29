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
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    const char json_text[] = "{\"name\":\"example\",\"nested\":{\"inner\":\"v\"}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", parsed);
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddStringToObject(extra, "note", "added");
    cJSON_AddItemToObject(root, "extra", extra);
    cJSON *data = cJSON_GetObjectItemCaseSensitive(root, "data");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(data, "name");
    cJSON *nested = cJSON_GetObjectItemCaseSensitive(data, "nested");
    cJSON *inner = cJSON_GetObjectItemCaseSensitive(nested, "inner");
    const char *extracted_name = cJSON_GetStringValue(name_item);
    const char *inner_value = cJSON_GetStringValue(inner);
    (void)extracted_name;
    (void)inner_value;
    cJSON_Delete(root);
    return 66;
}