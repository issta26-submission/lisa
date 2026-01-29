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
//<ID> 1162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json1 = "{\"num\": 42.5, \"name\": \"example\"}";
    size_t len1 = strlen(json1);
    cJSON *root1 = cJSON_ParseWithLength(json1, len1);
    cJSON *num_item = cJSON_GetObjectItem(root1, "num");
    double extracted_number = cJSON_GetNumberValue(num_item);

    // step 2: Configure
    cJSON *root2 = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw("raw_unparsed_segment");
    cJSON_AddItemToObject(root2, "raw", raw_item);
    cJSON_AddNumberToObject(root2, "copied_num", extracted_number);
    const char *json2 = "{\"opt\": 1}TRAILING";
    const char *parse_end = NULL;
    cJSON *parsed_opt = cJSON_ParseWithOpts(json2, &parse_end, 0);
    cJSON_AddItemToObject(root2, "parsed", parsed_opt);

    // step 3: Operate
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root2, "raw_is_raw", raw_check);
    cJSON *copied_item = cJSON_GetObjectItem(root2, "copied_num");
    double copied_value = cJSON_GetNumberValue(copied_item);
    cJSON_AddNumberToObject(root2, "copied_num_check", copied_value);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root2);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root2, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root1);
    cJSON_Delete(root2);

    // API sequence test completed successfully
    return 66;
}