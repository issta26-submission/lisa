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
//<ID> 2074
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *count_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateString("first"));

    // step 2: Configure
    char json_buf[] = " { \"extra\" : \"value\" , \"num\": 3.5 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *extra = cJSON_GetObjectItem(parsed, "extra");
    const char *extra_str = cJSON_GetStringValue(extra);
    cJSON *num_item = cJSON_GetObjectItem(parsed, "num");
    double parsed_num = cJSON_GetNumberValue(num_item);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(parsed_num));
    cJSON_AddItemToArray(items, cJSON_Duplicate(extra, 1));

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, root_dup, 1);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = name_str[0];
    buffer[2] = (char)('0' + ((int)count_val % 10));
    buffer[3] = extra_str[0];
    buffer[4] = (char)('0' + (int)equal);
    buffer[5] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}