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
//<ID> 1164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json1[] = "{\"num\": 7.5, \"rawdata\": \"<raw>text</raw>\"}";
    size_t len1 = sizeof(json1) - 1;
    cJSON *parsed1 = cJSON_ParseWithLength(json1, len1);
    const char json2[] = "{\"inner\": {\"value\": 2}} trailing";
    const char *parse_end = NULL;
    cJSON *parsed2 = cJSON_ParseWithOpts(json2, &parse_end, 0);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *num_item = cJSON_GetObjectItem(parsed1, "num");
    double extracted = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "extracted", extracted);
    cJSON *raw_item = cJSON_GetObjectItem(parsed1, "rawdata");
    cJSON_bool rawflag = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", rawflag);
    cJSON *inner = cJSON_GetObjectItem(parsed2, "inner");
    cJSON *inner_dup = cJSON_Duplicate(inner, 1);
    cJSON_AddItemToObject(root, "inner_dup", inner_dup);

    // step 3: Operate
    int ints[2] = { (int)extracted, 42 };
    cJSON *int_arr = cJSON_CreateIntArray(ints, 2);
    cJSON_AddItemToObject(root, "ints", int_arr);
    cJSON *rawnew = cJSON_CreateRaw("<raw>raw_content</raw>");
    cJSON_ReplaceItemViaPointer(root, int_arr, rawnew);
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(parsed1);
    cJSON_Delete(parsed2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}