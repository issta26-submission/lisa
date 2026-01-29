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
//<ID> 1167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_len[] = "{\"n\": 7, \"arr\": [1,2]}";
    const char json_opts[] = "{\"opt\": 42} trailing";
    size_t json_len_size = sizeof(json_len) - 1;
    const char *parse_end = NULL;
    cJSON *parsed_len = cJSON_ParseWithLength(json_len, json_len_size);
    cJSON *parsed_opts = cJSON_ParseWithOpts(json_opts, &parse_end, 0);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "from_len", parsed_len);
    cJSON_AddItemToObject(root, "from_opts", parsed_opts);
    cJSON *raw_item = cJSON_CreateRaw("RAW_CONTENT");
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi_item);

    // step 3: Operate
    cJSON *n_item = cJSON_GetObjectItem(parsed_len, "n");
    double n_val = cJSON_GetNumberValue(n_item);
    cJSON_AddNumberToObject(root, "n_copy", n_val);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_is_raw);
    cJSON *dup_opts = cJSON_Duplicate(parsed_opts, 1);
    cJSON_AddItemToObject(root, "dup_opts", dup_opts);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}