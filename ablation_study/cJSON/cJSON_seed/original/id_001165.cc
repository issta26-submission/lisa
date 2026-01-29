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
//<ID> 1165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"num\": 7.25, \"text\": \"hello\"}";
    size_t len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root1 = cJSON_ParseWithLength(json, len);
    cJSON *root2 = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *root3 = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObject(root3, "raw", raw_item);
    cJSON *num_item = cJSON_GetObjectItem(root1, "num");
    cJSON *num_dup = cJSON_Duplicate(num_item, 1);
    cJSON_AddItemToObject(root3, "num", num_dup);

    // step 3: Operate
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    double extracted_value = cJSON_GetNumberValue(num_dup);
    cJSON *flag_item = cJSON_CreateBool(raw_is_raw);
    cJSON_AddItemToObject(root3, "raw_is_raw", flag_item);
    cJSON *extracted_holder = cJSON_CreateNumber(extracted_value);
    cJSON_AddItemToObject(root3, "extracted_value", extracted_holder);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root3);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root3, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root1);
    cJSON_Delete(root2);
    cJSON_Delete(root3);

    // API sequence test completed successfully
    return 66;
}