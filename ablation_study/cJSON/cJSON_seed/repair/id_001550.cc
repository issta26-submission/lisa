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
//<ID> 1550
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json_text = "{\"parsed\":\"value\",\"count\":7}";
    cJSON *embedded_obj = cJSON_Parse(json_text);
    cJSON_AddItemToObjectCS(root, "embedded", embedded_obj);
    cJSON *name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObjectCS(root, "name", name_item);
    cJSON *num_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObjectCS(root, "num", num_item);

    // step 2: Configure
    cJSON *replacement_name = cJSON_CreateStringReference("replaced_name");
    cJSON_ReplaceItemViaPointer(root, name_item, replacement_name);

    // step 3: Operate and Validate
    cJSON *embedded = cJSON_GetObjectItem(root, "embedded");
    cJSON *parsed_value = cJSON_GetObjectItem(embedded, "parsed");
    cJSON_bool parsed_is_string = cJSON_IsString(parsed_value);
    cJSON *count_item = cJSON_GetObjectItem(embedded, "count");
    double original_num = cJSON_GetNumberValue(num_item);
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *sum_item = cJSON_CreateNumber(original_num + count_val);
    cJSON_AddItemToObjectCS(root, "sum", sum_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = (char)('0' + (int)parsed_is_string);
    scratch[1] = (char)('0' + (int)original_num);
    scratch[2] = (char)('0' + (int)count_val);
    scratch[3] = (char)('0' + (int)(original_num + count_val));
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}