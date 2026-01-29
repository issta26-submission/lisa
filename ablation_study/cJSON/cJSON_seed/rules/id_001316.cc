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
//<ID> 1316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"alpha\": 1, \"beta\": \"test\"}";
    size_t json_len = strlen(json);
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *manual = NULL;
    cJSON *arr = NULL;
    cJSON *raw_item = NULL;
    cJSON *beta_item = NULL;
    cJSON *arr_first = NULL;
    char *printed = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool cmp_beta = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    manual = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(manual, "beta");
    cJSON_AddItemToArray(arr, cJSON_CreateString("test"));
    raw_item = cJSON_CreateRaw("unparsed-raw-content");
    cJSON_AddItemToObject(manual, "raw", raw_item);

    // step 3: Operate / Validate
    beta_item = cJSON_GetObjectItem(parsed, "beta");
    arr_first = cJSON_GetArrayItem(arr, 0);
    is_raw = cJSON_IsRaw(raw_item);
    cmp_beta = cJSON_Compare(beta_item, arr_first, 1);
    printed = cJSON_PrintUnformatted(manual);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(manual);

    (void)is_raw;
    (void)cmp_beta;

    // API sequence test completed successfully
    return 66;
}