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
//<ID> 709
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
    cJSON *items = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item1 = cJSON_CreateString("alpha");
    cJSON *str_item2 = cJSON_CreateString("beta");

    // step 2: Configure
    cJSON_AddItemToArray(items, num_item);
    cJSON_AddItemToArray(items, str_item1);
    cJSON_AddItemToArray(items, str_item2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("example_name"));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "count", cJSON_CreateNumber(3.0));
    cJSON_AddItemToObject(root, "meta", meta);
    (void)version;

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *parsed_name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    const char *name_value = cJSON_GetStringValue(parsed_name);
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    double first_num = cJSON_GetNumberValue(first_item);
    cJSON *parsed_meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    cJSON *parsed_count = cJSON_GetObjectItemCaseSensitive(parsed_meta, "count");
    double count_val = cJSON_GetNumberValue(parsed_count);
    (void)name_value;
    (void)first_num;
    (void)count_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}