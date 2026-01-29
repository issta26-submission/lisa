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
//<ID> 2071
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name_item = cJSON_CreateString("unit_test");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *version_item = cJSON_CreateNumber(1.23);
    cJSON_AddItemToObject(meta, "version", version_item);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *first_elem = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, first_elem);

    // step 2: Configure
    char json_buf[] = " { \"name\" : \"parsed_name\" , \"value\": 42 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *p_name_item = cJSON_GetObjectItem(parsed, "name");
    const char *p_name_str = cJSON_GetStringValue(p_name_item);
    cJSON *p_value_item = cJSON_GetObjectItem(parsed, "value");
    double p_value_num = cJSON_GetNumberValue(p_value_item);
    cJSON_AddItemToObject(root, "parsed_name", cJSON_CreateString(p_name_str));
    cJSON_AddItemToObject(root, "parsed_value", cJSON_CreateNumber(p_value_num));

    // step 3: Operate and Validate
    cJSON *name_dup = cJSON_Duplicate(name_item, 1);
    cJSON_bool are_equal = cJSON_Compare(name_item, name_dup, 1);
    cJSON_AddItemToObject(root, "name_equal", cJSON_CreateBool(are_equal));
    char *printed = cJSON_PrintUnformatted(root);
    const char *got_parsed_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "parsed_name"));
    double got_parsed_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "parsed_value"));
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = got_parsed_name[0];
    buffer[2] = (char)('0' + ((int)got_parsed_value % 10));
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(name_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}