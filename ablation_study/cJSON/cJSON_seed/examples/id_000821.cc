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
//<ID> 821
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num_pi);
    cJSON *missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", missing);
    cJSON *raw_item = cJSON_CreateRaw("unparsed_raw");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 2: Configure
    cJSON_bool is_missing_null = cJSON_IsNull(missing);
    cJSON_bool is_raw_flag = cJSON_IsRaw(raw_item);
    double flag_value = (double)is_missing_null + ((double)is_raw_flag * 2.0);
    cJSON *flags = cJSON_CreateNumber(flag_value);
    cJSON_AddItemToObject(root, "flags", flags);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_flags = cJSON_GetObjectItem(parsed, "flags");
    double parsed_flag_value = cJSON_GetNumberValue(parsed_flags);
    (void)parsed_flag_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}