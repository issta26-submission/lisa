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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    size_t buf_len = 128;
    char *buf = (char *)cJSON_malloc(buf_len);
    memset(buf, 0, buf_len);
    const char *json_ws = "{ \"alpha\" : 123 , \"beta\" : null , \"arr\" : [ 1 , 2 , 3 ] }";
    memcpy(buf, json_ws, strlen(json_ws) + 1);
    cJSON_Minify(buf);
    cJSON *parsed = cJSON_Parse(buf);

    // step 2: Configure
    cJSON *manual = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(manual, "beta", null_item);
    cJSON *num_item = cJSON_CreateNumber(123);
    cJSON_AddItemToObject(manual, "alpha", num_item);
    int nums[3] = {1, 2, 3};
    cJSON *arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(manual, "arr", arr);

    // step 3: Operate and Validate
    char *printed_manual = cJSON_PrintUnformatted(manual);
    char *printed_parsed = cJSON_PrintUnformatted(parsed);
    cJSON_bool equal = cJSON_Compare(parsed, manual, 1);
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    double parsed_alpha = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "alpha"));
    (void)equal;
    (void)parsed_alpha;

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(printed_manual);
    cJSON_free(printed_parsed);
    cJSON_Delete(dup);
    cJSON_Delete(manual);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}