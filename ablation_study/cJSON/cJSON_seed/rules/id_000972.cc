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
//<ID> 972
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *raw_json = "{\"sensor\":\"temp\",\"value\":23.5,\"active\":true}";
    size_t raw_len = (size_t)(sizeof("{\"sensor\":\"temp\",\"value\":23.5,\"active\":true}") - 1);
    const char *parse_end = NULL;
    cJSON *constructed = NULL;
    cJSON *arr = NULL;
    cJSON *added_num1 = NULL;
    cJSON *added_num2 = NULL;
    cJSON *parsed = NULL;
    char *unfmt = NULL;
    char *prebuf = NULL;
    int prebuf_len = 256;
    cJSON_bool prealloc_fmt = 0;
    const char *version = NULL;
    double extracted_value = 0.0;
    cJSON_bool prealloc_res = 0;

    // step 2: Setup / Configure
    constructed = cJSON_CreateObject();
    cJSON_AddStringToObject(constructed, "source", "device");
    cJSON_AddNumberToObject(constructed, "id", 7.0);
    arr = cJSON_AddArrayToObject(constructed, "readings");
    added_num1 = cJSON_CreateNumber(1.1);
    cJSON_AddItemToArray(arr, added_num1);
    added_num2 = cJSON_CreateNumber(2.2);
    cJSON_AddItemToArray(arr, added_num2);
    cJSON_AddTrueToObject(constructed, "ok");
    parsed = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);

    // step 3: Operate / Validate
    version = cJSON_Version();
    unfmt = cJSON_PrintUnformatted(parsed);
    extracted_value = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "value"));
    prebuf = (char *)cJSON_malloc((size_t)prebuf_len);
    memset(prebuf, 0, (size_t)prebuf_len);
    prealloc_res = cJSON_PrintPreallocated(constructed, prebuf, prebuf_len, prealloc_fmt);

    // step 4: Cleanup
    cJSON_free(unfmt);
    cJSON_free(prebuf);
    cJSON_Delete(parsed);
    cJSON_Delete(constructed);

    // API sequence test completed successfully
    return 66;
}