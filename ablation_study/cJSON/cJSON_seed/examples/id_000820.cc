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
//<ID> 820
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "number", num);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "none", null_item);
    cJSON *raw_item = cJSON_CreateRaw("\"raw-string\"");
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_bool null_flag = cJSON_IsNull(null_item);
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON_AddItemToObject(root, "null_flag", cJSON_CreateNumber((double)null_flag));
    cJSON_AddItemToObject(root, "raw_flag", cJSON_CreateNumber((double)raw_flag));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    double original = cJSON_GetNumberValue(num);
    cJSON *num_plus_one = cJSON_CreateNumber(original + 1.0);
    cJSON_AddItemToObject(meta, "number_plus_one", num_plus_one);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_num_plus_one = cJSON_GetObjectItem(parsed_meta, "number_plus_one");
    double validated_value = cJSON_GetNumberValue(parsed_num_plus_one);
    cJSON *validated_copy = cJSON_CreateNumber(validated_value);
    cJSON_AddItemToObject(parsed_meta, "validated_copy", validated_copy);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}