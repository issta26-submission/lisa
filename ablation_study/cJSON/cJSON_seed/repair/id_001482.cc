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
//<ID> 1482
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"values\":[10,20,30],\"x\":42}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, (size_t)strlen(json), &parse_end, 1);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_GetObjectItemCaseSensitive(parsed, "values");
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double extracted = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "extracted", extracted);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(extracted));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(extracted + 1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(extracted + 2.0));
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_DeleteItemFromArray(arr, 1);

    // step 3: Operate and Validate
    cJSON *got = cJSON_GetObjectItemCaseSensitive(root, "extracted");
    double gotval = cJSON_GetNumberValue(got);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = (char)('0' + ((int)gotval % 10));
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('A');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}