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
//<ID> 1485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[1,2,3],\"meta\":{\"value\":42.5}}";
    size_t len = (size_t)strlen(json);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *value_item = cJSON_GetObjectItemCaseSensitive(meta, "value");
    double extracted = cJSON_GetNumberValue(value_item);
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");

    // step 2: Configure
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *report = cJSON_CreateObject();
    cJSON_AddNumberToObject(report, "extracted", extracted);
    cJSON_AddItemToObject(root, "report", report);

    // step 3: Operate and Validate
    cJSON *got_report = cJSON_GetObjectItemCaseSensitive(root, "report");
    cJSON *got_extracted = cJSON_GetObjectItemCaseSensitive(got_report, "extracted");
    double final_val = cJSON_GetNumberValue(got_extracted);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = (char)('0' + ((int)final_val % 10));
    scratch[1] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}